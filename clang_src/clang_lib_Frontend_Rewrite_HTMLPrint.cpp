//===--- HTMLPrint.cpp - Source code -> HTML pretty-printing --------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Pretty-printing of source code to HTML.
//
//===----------------------------------------------------------------------===//

#include "clang_include_clang_AST_ASTConsumer.h"
#include "clang_include_clang_AST_ASTContext.h"
#include "clang_include_clang_AST_Decl.h"
#include "clang_include_clang_Basic_Diagnostic.h"
#include "clang_include_clang_Basic_FileManager.h"
#include "clang_include_clang_Basic_SourceManager.h"
#include "clang_include_clang_Lex_Preprocessor.h"
#include "clang_include_clang_Rewrite_Core_HTMLRewrite.h"
#include "clang_include_clang_Rewrite_Core_Rewriter.h"
#include "clang_include_clang_Rewrite_Frontend_ASTConsumers.h"
#include "llvm_include_llvm_Support_raw_ostream.h"
using namespace clang;

//===----------------------------------------------------------------------===//
// Functional HTML pretty-printing.
//===----------------------------------------------------------------------===//

namespace {
  class HTMLPrinter : public ASTConsumer {
    Rewriter R;
    std::unique_ptr<raw_ostream> Out;
    Preprocessor &PP;
    bool SyntaxHighlight, HighlightMacros;

  public:
    HTMLPrinter(std::unique_ptr<raw_ostream> OS, Preprocessor &pp,
                bool _SyntaxHighlight, bool _HighlightMacros)
      : Out(std::move(OS)), PP(pp), SyntaxHighlight(_SyntaxHighlight),
        HighlightMacros(_HighlightMacros) {}

    void Initialize(ASTContext &context) override;
    void HandleTranslationUnit(ASTContext &Ctx) override;
  };
}

std::unique_ptr<ASTConsumer>
clang::CreateHTMLPrinter(std::unique_ptr<raw_ostream> OS, Preprocessor &PP,
                         bool SyntaxHighlight, bool HighlightMacros) {
  return std::make_unique<HTMLPrinter>(std::move(OS), PP, SyntaxHighlight,
                                        HighlightMacros);
}

void HTMLPrinter::Initialize(ASTContext &context) {
  R.setSourceMgr(context.getSourceManager(), context.getLangOpts());
}

void HTMLPrinter::HandleTranslationUnit(ASTContext &Ctx) {
  if (PP.getDiagnostics().hasErrorOccurred())
    return;

  // Format the file.
  FileID FID = R.getSourceMgr().getMainFileID();
  const FileEntry* Entry = R.getSourceMgr().getFileEntryForID(FID);
  StringRef Name;
  // In some cases, in particular the case where the input is from stdin,
  // there is no entry.  Fall back to the memory buffer for a name in those
  // cases.
  if (Entry)
    Name = Entry->getName();
  else
    Name = R.getSourceMgr().getBufferOrFake(FID).getBufferIdentifier();

  html::AddLineNumbers(R, FID);
  html::AddHeaderFooterInternalBuiltinCSS(R, FID, Name);

  // If we have a preprocessor, relex the file and syntax highlight.
  // We might not have a preprocessor if we come from a deserialized AST file,
  // for example.

  if (SyntaxHighlight) html::SyntaxHighlight(R, FID, PP);
  if (HighlightMacros) html::HighlightMacros(R, FID, PP);
  html::EscapeText(R, FID, false, true);

  // Emit the HTML.
  const RewriteBuffer &RewriteBuf = R.getEditBuffer(FID);
  std::unique_ptr<char[]> Buffer(new char[RewriteBuf.size()]);
  std::copy(RewriteBuf.begin(), RewriteBuf.end(), Buffer.get());
  Out->write(Buffer.get(), RewriteBuf.size());
}
