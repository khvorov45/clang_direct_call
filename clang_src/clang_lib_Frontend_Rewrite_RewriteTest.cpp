//===--- RewriteTest.cpp - Rewriter playground ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This is a testbed.
//
//===----------------------------------------------------------------------===//

#include "clang_include_clang_Lex_Preprocessor.h"
#include "clang_include_clang_Rewrite_Core_TokenRewriter.h"
#include "clang_include_clang_Rewrite_Frontend_Rewriters.h"
#include "llvm_include_llvm_Support_raw_ostream.h"

void clang::DoRewriteTest(Preprocessor &PP, raw_ostream *OS) {
  SourceManager &SM = PP.getSourceManager();
  const LangOptions &LangOpts = PP.getLangOpts();

  TokenRewriter Rewriter(SM.getMainFileID(), SM, LangOpts);

  // Throw <i> </i> tags around comments.
  for (TokenRewriter::token_iterator I = Rewriter.token_begin(),
       E = Rewriter.token_end(); I != E; ++I) {
    if (I->isNot(tok::comment)) continue;

    Rewriter.AddTokenBefore(I, "<i>");
    Rewriter.AddTokenAfter(I, "</i>");
  }


  // Print out the output.
  for (TokenRewriter::token_iterator I = Rewriter.token_begin(),
       E = Rewriter.token_end(); I != E; ++I)
    *OS << PP.getSpelling(*I);
}
