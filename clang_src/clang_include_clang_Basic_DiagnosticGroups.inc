
#ifdef GET_DIAG_ARRAYS
static const int16_t DiagArrays[] = {
  /* Empty */ -1,
  /* DiagArray1 */ diag::warn_pragma_message, -1,
  /* DiagArray2 */ diag::pp_hash_warning, -1,
  /* DiagArray3 */ diag::warn_cfstring_truncated, -1,
  /* DiagArray5 */ diag::warn_independentclass_attribute, diag::warn_ptr_independentclass_attribute, -1,
  /* DiagArray6 */ diag::warn_nsobject_attribute, -1,
  /* DiagArray8 */ diag::warn_abs_too_small, diag::warn_pointer_abs, diag::warn_unsigned_abs, diag::warn_wrong_absolute_value_type, -1,
  /* DiagArray9 */ diag::warn_abstract_final_class, -1,
  /* DiagArray10 */ diag::warn_abstract_vbase_init_ignored, -1,
  /* DiagArray12 */ diag::warn_taking_address_of_packed_member, -1,
  /* DiagArray13 */ diag::ext_typecheck_addrof_temporary, -1,
  /* DiagArray15 */ diag::warn_not_xl_compatible, diag::warn_pragma_align_not_xl_compatible, -1,
  /* DiagArray16 */ diag::warn_param_mismatched_alignment, -1,
  /* DiagArray18 */ diag::warn_alloca, -1,
  /* DiagArray19 */ diag::warn_alloca_align_alignof, -1,
  /* DiagArray20 */ diag::warn_always_inline_coroutine, -1,
  /* DiagArray21 */ diag::warn_ambiguous_suitable_delete_function_found, -1,
  /* DiagArray22 */ diag::warn_misplaced_ellipsis_vararg, -1,
  /* DiagArray23 */ diag::warn_pp_ambiguous_macro, -1,
  /* DiagArray24 */ diag::ext_nested_name_member_ref_lookup_ambiguous, -1,
  /* DiagArray25 */ diag::ext_ovl_ambiguous_oper_binary_reversed, -1,
  /* DiagArray26 */ diag::warn_incompatible_analyzer_plugin_api, -1,
  /* DiagArray27 */ diag::warn_arith_conv_mixed_anon_enum_types, -1,
  /* DiagArray28 */ diag::ext_abstract_pack_declarator_parens, -1,
  /* DiagArray30 */ diag::warn_arc_bridge_cast_nonarc, -1,
  /* DiagArray31 */ diag::warn_arc_possible_repeated_use_of_weak, -1,
  /* DiagArray32 */ diag::warn_arc_object_memaccess, -1,
  /* DiagArray33 */ diag::warn_arc_perform_selector_leaks, -1,
  /* DiagArray34 */ diag::warn_arc_repeated_use_of_weak, -1,
  /* DiagArray35 */ diag::warn_arc_retain_cycle, -1,
  /* DiagArray36 */ diag::warn_arc_literal_assign, diag::warn_arc_retained_assign, diag::warn_arc_retained_property_assign, -1,
  /* DiagArray37 */ diag::warn_argument_invalid_range, -1,
  /* DiagArray38 */ diag::warn_argument_undefined_behaviour, -1,
  /* DiagArray39 */ diag::warn_array_index_exceeds_bounds, diag::warn_array_index_exceeds_max_addressable_bounds, diag::warn_array_index_precedes_bounds, diag::warn_ptr_arith_exceeds_max_addressable_bounds, diag::warn_static_array_too_small, -1,
  /* DiagArray40 */ diag::warn_ptr_arith_exceeds_bounds, diag::warn_ptr_arith_precedes_bounds, -1,
  /* DiagArray41 */ diag::warn_inconsistent_array_form, -1,
  /* DiagArray43 */ diag::warn_asm_mismatched_size_modifier, -1,
  /* DiagArray44 */ diag::warn_not_in_enum_assignment, -1,
  /* DiagArray45 */ diag::warn_assume_side_effects, -1,
  /* DiagArray47 */ diag::warn_atimport_in_framework_header, -1,
  /* DiagArray48 */ diag::warn_atomic_member_access, -1,
  /* DiagArray49 */ diag::warn_atomic_op_misaligned, diag::warn_atomic_op_oversized, -1,
  /* DiagArray50 */ diag::warn_atomic_implicit_seq_cst, -1,
  /* DiagArray51 */ diag::warn_atomic_op_has_invalid_memory_order, -1,
  /* DiagArray53 */ diag::warn_atomic_property_rule, -1,
  /* DiagArray54 */ diag::warn_attribute_packed_for_bitfield, -1,
  /* DiagArray55 */ diag::warn_fe_backend_warning_attr, -1,
  /* DiagArray57 */ diag::warn_drv_disabling_vptr_no_rtti_default, -1,
  /* DiagArray59 */ diag::ext_auto_storage_class, -1,
  /* DiagArray60 */ diag::warn_auto_var_is_id, -1,
  /* DiagArray61 */ diag::warn_availability_and_unavailable, diag::warn_availability_fuchsia_unavailable_minor, diag::warn_availability_on_static_initializer, diag::warn_availability_swift_unavailable_deprecated_only, diag::warn_availability_unknown_platform, diag::warn_availability_version_ordering, diag::warn_expected_consistent_version_separator, diag::warn_mismatched_availability, diag::warn_mismatched_availability_override, diag::warn_mismatched_availability_override_unavail, -1,
  /* DiagArray62 */ diag::warn_drv_avr_family_linking_stdlibs_not_implemented, diag::warn_drv_avr_libc_not_found, diag::warn_drv_avr_linker_section_addresses_not_implemented, diag::warn_drv_avr_mcu_not_specified, diag::warn_drv_avr_stdlib_not_linked, -1,
  /* DiagArray63 */ diag::warn_fe_backend_plugin, diag::warn_fe_backend_resource_limit, -1,
  /* DiagArray64 */ diag::backslash_newline_space, -1,
  /* DiagArray65 */ diag::warn_bad_function_cast, -1,
  /* DiagArray67 */ diag::ext_rvalue_to_reference_access_ctor, diag::ext_rvalue_to_reference_temp_copy_no_viable, -1,
  /* DiagArray68 */ diag::ext_decomp_decl_cond, -1,
  /* DiagArray69 */ diag::ext_bit_int, -1,
  /* DiagArray70 */ diag::warn_impcast_bitfield_precision_constant, -1,
  /* DiagArray71 */ diag::warn_bitfield_too_small_for_enum, diag::warn_signed_bitfield_enum_conversion, diag::warn_unsigned_bitfield_assigned_signed_enum, -1,
  /* DiagArray72 */ diag::warn_bitfield_width_exceeds_type_width, -1,
  /* DiagArray73 */ diag::warn_precedence_bitwise_conditional, -1,
  /* DiagArray74 */ diag::warn_bitwise_instead_of_logical, -1,
  /* DiagArray75 */ diag::warn_bitwise_op_in_bitwise_op, -1,
  /* DiagArray76 */ diag::warn_block_capture_autoreleasing, -1,
  /* DiagArray77 */ diag::warn_impcast_bool_to_null_pointer, -1,
  /* DiagArray79 */ diag::warn_bitwise_negation_bool, -1,
  /* DiagArray80 */ diag::warn_braces_around_init, -1,
  /* DiagArray81 */ diag::warn_incompatible_branch_protection_option, diag::warn_target_unsupported_branch_protection_attribute, diag::warn_unsupported_branch_protection, diag::warn_unsupported_branch_protection_spec, -1,
  /* DiagArray82 */ diag::warn_objc_invalid_bridge, diag::warn_objc_invalid_bridge_to_cf, -1,
  /* DiagArray83 */ diag::warn_assume_aligned_too_great, -1,
  /* DiagArray84 */ diag::ext_pp_redef_builtin_macro, diag::ext_pp_undef_builtin_macro, -1,
  /* DiagArray85 */ diag::warn_builtin_chk_overflow, -1,
  /* DiagArray86 */ diag::warn_implicit_decl_requires_sysheader, -1,
  /* DiagArray87 */ diag::warn_zero_size_struct_union_compat, -1,
  /* DiagArray91 */ diag::ext_old_implicitly_unsigned_long_cxx, diag::warn_auto_storage_class, diag::warn_cxx11_compat_user_defined_literal, diag::warn_cxx11_keyword, diag::warn_cxx11_right_shift_in_template_arg, diag::warn_explicit_instantiation_inline_0x, diag::warn_explicit_instantiation_must_be_global_0x, diag::warn_explicit_instantiation_out_of_scope_0x, diag::warn_explicit_instantiation_unqualified_wrong_namespace_0x, diag::warn_old_implicitly_unsigned_long_cxx, -1,
  /* DiagArray92 */ diag::warn_deprecated_string_literal_conversion, -1,
  /* DiagArray94 */ diag::warn_cxx11_compat_reserved_user_defined_literal, -1,
  /* DiagArray95 */ diag::ext_alias_declaration, diag::ext_array_size_conversion, diag::ext_auto_type_specifier, diag::ext_cxx11_enum_fixed_underlying_type, diag::ext_defaulted_deleted_function, diag::ext_enum_friend, diag::ext_enumerator_list_comma_cxx, diag::ext_explicit_conversion_functions, diag::ext_extern_template, diag::ext_for_range, diag::ext_generalized_initializer_lists, diag::ext_nested_name_spec_is_enum, diag::ext_nonclass_type_friend, diag::ext_nonstatic_member_init, diag::ext_override_control_keyword, diag::ext_ref_qualifier, diag::ext_rvalue_reference, diag::ext_scoped_enum, diag::ext_static_data_member_in_union, diag::ext_template_arg_object_internal, diag::ext_template_outside_of_template, diag::ext_template_parameter_default_in_function_template, diag::ext_typename_outside_of_template, diag::ext_unelaborated_friend_type, diag::ext_variadic_templates, -1,
  /* DiagArray96 */ diag::ext_extra_semi_cxx11, -1,
  /* DiagArray97 */ diag::ext_inline_namespace, -1,
  /* DiagArray98 */ diag::ext_cxx11_longlong, -1,
  /* DiagArray99 */ diag::ext_cce_narrowing, diag::ext_init_list_constant_narrowing, diag::ext_init_list_type_narrowing, diag::ext_init_list_variable_narrowing, diag::warn_init_list_constant_narrowing, diag::warn_init_list_type_narrowing, diag::warn_init_list_variable_narrowing, -1,
  /* DiagArray100 */ diag::ext_cxx14_attr, -1,
  /* DiagArray101 */ diag::ext_binary_literal_cxx14, -1,
  /* DiagArray104 */ diag::ext_constexpr_body_invalid_stmt, diag::ext_constexpr_body_multiple_return, diag::ext_constexpr_local_var, diag::ext_constexpr_type_definition, diag::ext_decltype_auto_type_specifier, diag::ext_init_capture, diag::ext_variable_template, -1,
  /* DiagArray105 */ diag::ext_cxx17_attr, -1,
  /* DiagArray107 */ diag::warn_cxx17_compat_exception_spec_in_signature, -1,
  /* DiagArray109 */ diag::ext_auto_new_list_init, diag::ext_constexpr_if, diag::ext_constexpr_on_lambda_cxx17, diag::ext_cxx_static_assert_no_message, diag::ext_decomp_decl, diag::ext_fold_expression, diag::ext_for_range_begin_end_types_differ, diag::ext_hex_literal_invalid, diag::ext_init_statement, diag::ext_inline_variable, diag::ext_multi_using_declaration, diag::ext_nested_namespace_definition, diag::ext_ns_enum_attribute, diag::ext_star_this_lambda_capture_cxx17, diag::ext_template_template_param_typename, diag::ext_using_attribute_ns, diag::ext_using_declaration_pack, -1,
  /* DiagArray114 */ diag::ext_cxx20_attr, -1,
  /* DiagArray115 */ diag::warn_cxx17_compat_implicit_typename, diag::warn_cxx20_compat_aggregate_init_with_ctors, diag::warn_cxx20_compat_consteval, diag::warn_cxx20_compat_constinit, diag::warn_cxx20_compat_explicit_bool, diag::warn_cxx20_compat_spaceship, diag::warn_cxx20_compat_use_of_unaddressable_function, diag::warn_cxx20_compat_utf8_string, diag::warn_cxx20_keyword, -1,
  /* DiagArray117 */ diag::ext_cxx_designated_init, -1,
  /* DiagArray118 */ diag::ext_adl_only_template_id, diag::ext_bitfield_member_init, diag::ext_capture_binding, diag::ext_constexpr_body_invalid_stmt_cxx20, diag::ext_constexpr_ctor_missing_init, diag::ext_constexpr_function_try_block_cxx20, diag::ext_constexpr_local_var_no_init, diag::ext_constexpr_union_ctor_no_init, diag::ext_decomp_decl_spec, diag::ext_defaulted_comparison, diag::ext_equals_this_lambda_capture_cxx20, diag::ext_explicit_bool, diag::ext_for_range_init_stmt, diag::ext_implicit_typename, diag::ext_init_capture_pack, diag::ext_inline_nested_namespace_definition, diag::ext_lambda_template_parameter_list, diag::ext_pointer_to_const_ref_member_on_rvalue, diag::ext_using_decl_scoped_enumerator, diag::ext_using_enum_declaration, -1,
  /* DiagArray122 */ diag::err_static_lambda, diag::ext_alias_in_init_statement, diag::ext_consteval_if, diag::ext_constexpr_body_invalid_stmt_cxx2b, diag::ext_constexpr_static_var, diag::ext_cxx2b_pp_directive, diag::ext_cxx2b_size_t_suffix, diag::ext_cxx_label_end_of_compound_statement, diag::ext_decl_attrs_on_lambda, diag::ext_lambda_missing_parens, diag::ext_operator_overload_static, -1,
  /* DiagArray128 */ diag::warn_cxx11_compat_binary_literal, -1,
  /* DiagArray130 */ diag::warn_cxx98_compat_alias_declaration, diag::warn_cxx98_compat_alignas, diag::warn_cxx98_compat_alignof, diag::warn_cxx98_compat_attribute, diag::warn_cxx98_compat_auto_type_specifier, diag::warn_cxx98_compat_constexpr, diag::warn_cxx98_compat_ctor_list_init, diag::warn_cxx98_compat_decltype, diag::warn_cxx98_compat_defaulted_deleted_function, diag::warn_cxx98_compat_delegating_ctor, diag::warn_cxx98_compat_empty_scalar_initializer, diag::warn_cxx98_compat_empty_sizeless_initializer, diag::warn_cxx98_compat_enum_fixed_underlying_type, diag::warn_cxx98_compat_enum_friend, diag::warn_cxx98_compat_enum_nested_name_spec, diag::warn_cxx98_compat_explicit_conversion_functions, diag::warn_cxx98_compat_for_range, diag::warn_cxx98_compat_friend_is_member, diag::warn_cxx98_compat_generalized_initializer_lists, diag::warn_cxx98_compat_goto_into_protected_scope, diag::warn_cxx98_compat_indirect_goto_in_protected_scope, diag::warn_cxx98_compat_initializer_list_init, diag::warn_cxx98_compat_inline_namespace, diag::warn_cxx98_compat_lambda, diag::warn_cxx98_compat_less_colon_colon, diag::warn_cxx98_compat_literal_operator, diag::warn_cxx98_compat_literal_ucn_control_character, diag::warn_cxx98_compat_literal_ucn_escape_basic_scs, diag::warn_cxx98_compat_noexcept_decl, diag::warn_cxx98_compat_noexcept_expr, diag::warn_cxx98_compat_non_static_member_use, diag::warn_cxx98_compat_nonclass_type_friend, diag::warn_cxx98_compat_nonstatic_member_init, diag::warn_cxx98_compat_nontrivial_union_or_anon_struct_member, diag::warn_cxx98_compat_nullptr, diag::warn_cxx98_compat_override_control_keyword, diag::warn_cxx98_compat_pass_non_pod_arg_to_vararg, diag::warn_cxx98_compat_raw_string_literal, diag::warn_cxx98_compat_ref_qualifier, diag::warn_cxx98_compat_reference_list_init, diag::warn_cxx98_compat_rvalue_reference, diag::warn_cxx98_compat_scoped_enum, diag::warn_cxx98_compat_sfinae_access_control, diag::warn_cxx98_compat_static_assert, diag::warn_cxx98_compat_static_data_member_in_union, diag::warn_cxx98_compat_switch_into_protected_scope, diag::warn_cxx98_compat_template_arg_extra_parens, diag::warn_cxx98_compat_template_arg_null, diag::warn_cxx98_compat_template_arg_object_internal, diag::warn_cxx98_compat_template_outside_of_template, diag::warn_cxx98_compat_template_parameter_default_in_function_template, diag::warn_cxx98_compat_trailing_return_type, diag::warn_cxx98_compat_two_right_angle_brackets, diag::warn_cxx98_compat_typename_outside_of_template, diag::warn_cxx98_compat_unelaborated_friend_type, diag::warn_cxx98_compat_unicode_literal, diag::warn_cxx98_compat_unicode_type, diag::warn_cxx98_compat_using_decl_constructor, diag::warn_cxx98_compat_variadic_templates, -1,
  /* DiagArray131 */ diag::warn_cxx98_compat_temp_copy, -1,
  /* DiagArray132 */ diag::warn_cxx98_compat_top_level_semi, -1,
  /* DiagArray133 */ diag::warn_cxx98_compat_template_arg_local_type, -1,
  /* DiagArray134 */ diag::warn_cxx98_compat_array_size_conversion, diag::warn_cxx98_compat_cast_fn_obj, diag::warn_cxx98_compat_empty_fnmacro_arg, diag::warn_cxx98_compat_enumerator_list_comma, diag::warn_cxx98_compat_extern_template, diag::warn_cxx98_compat_longlong, diag::warn_cxx98_compat_no_newline_eof, diag::warn_cxx98_compat_pp_line_too_big, diag::warn_cxx98_compat_variadic_macro, -1,
  /* DiagArray135 */ diag::warn_cxx98_compat_template_arg_unnamed_type, -1,
  /* DiagArray136 */ diag::ext_anonymous_union, diag::ext_c11_anonymous_struct, diag::ext_c11_feature, diag::ext_typecheck_compare_complete_incomplete_pointers, -1,
  /* DiagArray137 */ diag::warn_c2x_keyword, -1,
  /* DiagArray138 */ diag::ext_c2x_bitint_suffix, diag::ext_c2x_pp_directive, diag::ext_c_label_end_of_compound_statement, diag::ext_c_nullptr, diag::ext_c_static_assert_no_message, diag::ext_parameter_name_omitted_c2x, -1,
  /* DiagArray139 */ diag::warn_c99_compat_unicode_id, diag::warn_c99_compat_unicode_literal, diag::warn_c99_keyword, diag::warn_old_implicitly_unsigned_long, -1,
  /* DiagArray140 */ diag::ext_designated_init, diag::ext_designated_init_array, diag::ext_designated_init_brace_elision, diag::ext_designated_init_mixed, diag::ext_designated_init_nested, -1,
  /* DiagArray141 */ diag::ext_aggregate_init_not_constant, diag::ext_c99_array_usage, diag::ext_c99_compound_literal, diag::ext_c99_feature, diag::ext_c99_flexible_array_member, diag::ext_c99_variable_decl_in_for_loop, diag::ext_c99_whitespace_required_after_macro_name, diag::ext_empty_fnmacro_arg, diag::ext_enumerator_list_comma_c, diag::ext_hex_constant_invalid, -1,
  /* DiagArray142 */ diag::warn_call_to_pure_virtual_member_function_from_ctor_dtor, -1,
  /* DiagArray143 */ diag::warn_called_once_gets_called_twice, diag::warn_called_once_never_called, diag::warn_called_once_never_called_when, -1,
  /* DiagArray144 */ diag::warn_cast_align, -1,
  /* DiagArray145 */ diag::warn_cast_calling_conv, -1,
  /* DiagArray146 */ diag::warn_cast_function_type, -1,
  /* DiagArray147 */ diag::warn_cast_function_type_strict, -1,
  /* DiagArray148 */ diag::warn_cast_pointer_from_sel, -1,
  /* DiagArray149 */ diag::warn_cast_qual, diag::warn_cast_qual2, -1,
  /* DiagArray150 */ diag::ext_bad_cxx_cast_qualifiers_away_incoherent, -1,
  /* DiagArray152 */ diag::warn_omp_section_is_char, diag::warn_subscript_is_char, -1,
  /* DiagArray153 */ diag::warn_drv_yc_multiple_inputs_clang_cl, diag::warn_drv_ycyu_different_arg_clang_cl, diag::warn_pp_hdrstop_filename_ignored, diag::warn_pp_macro_def_mismatch_with_pch, -1,
  /* DiagArray154 */ diag::warn_conv_to_base_not_used, diag::warn_conv_to_self_not_used, diag::warn_conv_to_void_not_used, -1,
  /* DiagArray155 */ diag::warn_pass_class_arg_to_vararg, -1,
  /* DiagArray156 */ diag::warn_cmse_nonsecure_union, -1,
  /* DiagArray157 */ diag::warn_comma_operator, -1,
  /* DiagArray158 */ diag::escaped_newline_block_comment_end, diag::ext_line_comment, diag::ext_multi_line_line_comment, diag::warn_nested_block_comment, -1,
  /* DiagArray160 */ diag::ext_typecheck_comparison_of_distinct_pointers, -1,
  /* DiagArray161 */ diag::warn_completion_handler_called_twice, diag::warn_completion_handler_never_called, diag::warn_completion_handler_never_called_when, -1,
  /* DiagArray162 */ diag::ext_complex_component_init, -1,
  /* DiagArray164 */ diag::warn_compound_token_split_by_macro, -1,
  /* DiagArray165 */ diag::warn_compound_token_split_by_whitespace, -1,
  /* DiagArray166 */ diag::ext_typecheck_cond_pointer_integer_mismatch, -1,
  /* DiagArray167 */ diag::warn_maybe_uninit_var, -1,
  /* DiagArray168 */ diag::warn_module_config_macro_undef, -1,
  /* DiagArray169 */ diag::warn_impcast_integer_precision_constant, -1,
  /* DiagArray170 */ diag::warn_is_constant_evaluated_always_true_constexpr, -1,
  /* DiagArray171 */ diag::warn_logical_instead_of_bitwise, -1,
  /* DiagArray172 */ diag::warn_cxx14_compat_constexpr_not_const, -1,
  /* DiagArray173 */ diag::warn_attr_on_unconsumable_class, diag::warn_loop_state_mismatch, diag::warn_param_return_typestate_mismatch, diag::warn_param_typestate_mismatch, diag::warn_return_typestate_for_unconsumable_type, diag::warn_return_typestate_mismatch, diag::warn_use_in_invalid_state, diag::warn_use_of_temp_in_invalid_state, -1,
  /* DiagArray174 */ diag::warn_impcast_complex_scalar, diag::warn_impcast_vector_scalar, diag::warn_opencl_generic_address_space_arg, diag::warn_template_arg_negative, diag::warn_template_arg_too_large, -1,
  /* DiagArray176 */ diag::warn_non_aligned_allocation_function, -1,
  /* DiagArray177 */ diag::warn_coroutine_handle_address_invalid_return_type, -1,
  /* DiagArray178 */ diag::warn_coroutine_promise_unhandled_exception_required_with_exceptions, -1,
  /* DiagArray179 */ diag::warn_unreachable_default, -1,
  /* DiagArray181 */ diag::warn_objc_cdirective_format_string, -1,
  /* DiagArray182 */ diag::warn_ctad_maybe_unsupported, -1,
  /* DiagArray184 */ diag::warn_ctu_incompat_triple, -1,
  /* DiagArray185 */ diag::warn_attribute_argument_n_negative, diag::warn_cuda_attr_lambda_position, diag::warn_kern_is_inline, diag::warn_kern_is_method, diag::warn_pragma_unroll_cuda_value_in_parens, -1,
  /* DiagArray186 */ diag::warn_default_atomic_custom_getter_setter, -1,
  /* DiagArray187 */ diag::ext_cxx11_attr_placement, -1,
  /* DiagArray188 */ diag::warn_dangling_variable, diag::warn_unsupported_lifetime_extension, -1,
  /* DiagArray189 */ diag::warn_dangling_else, -1,
  /* DiagArray190 */ diag::warn_bind_ref_member_to_parameter, diag::warn_dangling_member, diag::warn_init_ptr_member_to_parameter_addr, diag::warn_new_dangling_reference, -1,
  /* DiagArray191 */ diag::warn_dangling_lifetime_pointer, diag::warn_dangling_lifetime_pointer_member, -1,
  /* DiagArray192 */ diag::warn_new_dangling_initializer_list, -1,
  /* DiagArray193 */ diag::warn_drv_darwin_sdk_invalid_settings, -1,
  /* DiagArray194 */ diag::warn_pp_date_time, -1,
  /* DiagArray195 */ diag::warn_dealloc_in_category, -1,
  /* DiagArray196 */ diag::warn_debug_compression_unavailable, -1,
  /* DiagArray197 */ diag::ext_mixed_decls_code, diag::warn_mixed_decls_code, -1,
  /* DiagArray198 */ diag::warn_defaulted_comparison_deleted, diag::warn_defaulted_method_deleted, -1,
  /* DiagArray199 */ diag::warn_delegating_ctor_cycle, -1,
  /* DiagArray200 */ diag::warn_delete_abstract_non_virtual_dtor, -1,
  /* DiagArray201 */ diag::ext_delete_void_ptr_operand, diag::warn_delete_incomplete, -1,
  /* DiagArray202 */ diag::warn_delete_non_virtual_dtor, -1,
  /* DiagArray204 */ diag::ext_delimited_escape_sequence, -1,
  /* DiagArray205 */ diag::warn_deprecated_lax_vec_conv_all, -1,
  /* DiagArray206 */ diag::warn_O4_is_O3, diag::warn_access_decl_deprecated, diag::warn_deprecated_redundant_constexpr_static_def, diag::warn_drv_deprecated_arg, diag::warn_drv_treating_input_as_cxx, diag::warn_option_invalid_ocl_version, diag::warn_vector_long_decl_spec_combination, -1,
  /* DiagArray207 */ diag::warn_deprecated_altivec_src_compat, -1,
  /* DiagArray208 */ diag::warn_arith_conv_mixed_anon_enum_types_cxx20, -1,
  /* DiagArray209 */ diag::warn_depr_array_comparison, -1,
  /* DiagArray210 */ diag::warn_deprecated_noreturn_spelling, diag::warn_type_attribute_deprecated_on_decl, diag::warn_vector_mode_deprecated, -1,
  /* DiagArray211 */ diag::warn_deprecated_builtin, -1,
  /* DiagArray212 */ diag::warn_deprecated_comma_subscript, -1,
  /* DiagArray213 */ diag::warn_deprecated_copy, -1,
  /* DiagArray215 */ diag::warn_deprecated_copy_with_dtor, -1,
  /* DiagArray216 */ diag::warn_deprecated_copy_with_user_provided_copy, -1,
  /* DiagArray217 */ diag::warn_deprecated_copy_with_user_provided_dtor, -1,
  /* DiagArray218 */ diag::warn_deprecated_for_co_await, -1,
  /* DiagArray219 */ diag::warn_atl_uuid_deprecated, diag::warn_cstyle_param, diag::warn_deprecated, diag::warn_deprecated_fwdclass_message, diag::warn_deprecated_message, diag::warn_property_method_deprecated, -1,
  /* DiagArray220 */ diag::warn_exception_spec_deprecated, -1,
  /* DiagArray221 */ diag::warn_comparison_mixed_enum_types_cxx20, -1,
  /* DiagArray222 */ diag::warn_conditional_mixed_enum_types_cxx20, -1,
  /* DiagArray223 */ diag::warn_arith_conv_mixed_enum_types_cxx20, -1,
  /* DiagArray224 */ diag::warn_arith_conv_enum_float_cxx20, -1,
  /* DiagArray225 */ diag::warn_deprecated_coroutine_namespace, -1,
  /* DiagArray226 */ diag::warn_deprecated_def, diag::warn_unavailable_def, -1,
  /* DiagArray227 */ diag::warn_increment_bool, -1,
  /* DiagArray228 */ diag::warn_non_prototype_changes_behavior, diag::warn_strict_uses_without_prototype, -1,
  /* DiagArray229 */ diag::warn_objc_isa_assign, diag::warn_objc_isa_use, -1,
  /* DiagArray230 */ diag::warn_objc_pointer_masking, -1,
  /* DiagArray231 */ diag::warn_objc_pointer_masking_performSelector, -1,
  /* DiagArray232 */ diag::warn_pragma_deprecated_macro_use, -1,
  /* DiagArray233 */ diag::warn_deprecated_register, -1,
  /* DiagArray234 */ diag::warn_analyzer_deprecated_option, diag::warn_analyzer_deprecated_option_with_alternative, -1,
  /* DiagArray235 */ diag::warn_deprecated_this_capture, -1,
  /* DiagArray236 */ diag::warn_ext_int_deprecated, -1,
  /* DiagArray237 */ diag::warn_deprecated_increment_decrement_volatile, diag::warn_deprecated_simple_assign_volatile, diag::warn_deprecated_volatile_param, diag::warn_deprecated_volatile_return, diag::warn_deprecated_volatile_structured_binding, -1,
  /* DiagArray239 */ diag::warn_direct_ivar_access, -1,
  /* DiagArray240 */ diag::pp_disabled_macro_expansion, -1,
  /* DiagArray243 */ diag::warn_conflicting_param_modifiers, diag::warn_conflicting_ret_type_modifiers, -1,
  /* DiagArray245 */ diag::warn_remainder_division_by_zero, -1,
  /* DiagArray246 */ diag::warn_attribute_dll_redeclaration, -1,
  /* DiagArray247 */ diag::warn_attribute_dllexport_explicit_instantiation_decl, -1,
  /* DiagArray248 */ diag::warn_attribute_dllimport_static_field_definition, -1,
  /* DiagArray249 */ diag::warn_doc_api_container_decl_mismatch, diag::warn_doc_block_command_duplicate, diag::warn_doc_block_command_empty_paragraph, diag::warn_doc_container_decl_mismatch, diag::warn_doc_function_method_decl_mismatch, diag::warn_doc_html_start_tag_expected_ident_or_greater, diag::warn_doc_html_start_tag_expected_quoted_string, diag::warn_doc_inline_command_not_enough_arguments, diag::warn_doc_param_duplicate, diag::warn_doc_param_invalid_direction, diag::warn_doc_param_not_attached_to_a_function_decl, diag::warn_doc_param_not_found, diag::warn_doc_returns_attached_to_a_void_function, diag::warn_doc_returns_not_attached_to_a_function_decl, diag::warn_doc_tparam_duplicate, diag::warn_doc_tparam_not_attached_to_a_template_decl, diag::warn_doc_tparam_not_found, diag::warn_not_a_doxygen_trailing_member_comment, diag::warn_verbatim_block_end_without_start, -1,
  /* DiagArray250 */ diag::warn_doc_deprecated_not_sync, -1,
  /* DiagArray251 */ diag::warn_doc_html_end_forbidden, diag::warn_doc_html_end_unbalanced, diag::warn_doc_html_missing_end_tag, diag::warn_doc_html_start_end_mismatch, -1,
  /* DiagArray252 */ diag::warn_doc_param_spaces_in_direction, -1,
  /* DiagArray253 */ diag::warn_correct_comment_command_name, diag::warn_unknown_comment_command_name, -1,
  /* DiagArray254 */ diag::ext_dollar_in_identifier, -1,
  /* DiagArray255 */ diag::warn_impcast_double_promotion, -1,
  /* DiagArray256 */ diag::ext_dtor_name_ambiguous, diag::ext_dtor_named_in_wrong_scope, diag::ext_qualified_dtor_named_in_lexical_scope, -1,
  /* DiagArray257 */ diag::ext_destructor_typedef_name, -1,
  /* DiagArray258 */ diag::ext_duplicate_declspec, diag::ext_warn_duplicate_declspec, diag::warn_attribute_address_multiple_identical_qualifiers, diag::warn_duplicate_declspec, -1,
  /* DiagArray259 */ diag::warn_duplicate_enum_values, -1,
  /* DiagArray260 */ diag::warn_method_param_declaration, -1,
  /* DiagArray261 */ diag::warn_duplicate_method_decl, -1,
  /* DiagArray262 */ diag::warn_duplicate_protocol_def, -1,
  /* DiagArray263 */ diag::warn_dyn_class_memaccess, -1,
  /* DiagArray264 */ diag::ext_dynamic_exception_spec, -1,
  /* DiagArray266 */ diag::ext_enum_base_in_type_specifier, -1,
  /* DiagArray267 */ diag::ext_elaborated_enum_class, -1,
  /* DiagArray268 */ diag::ext_embedded_directive, -1,
  /* DiagArray269 */ diag::warn_empty_for_body, diag::warn_empty_if_body, diag::warn_empty_range_based_for_body, diag::warn_empty_switch_body, diag::warn_empty_while_body, -1,
  /* DiagArray270 */ diag::ext_decomp_decl_empty, -1,
  /* DiagArray271 */ diag::warn_empty_init_statement, -1,
  /* DiagArray272 */ diag::ext_empty_translation_unit, -1,
  /* DiagArray273 */ diag::warn_incomplete_encoded_type, -1,
  /* DiagArray275 */ diag::warn_comparison_mixed_enum_types, -1,
  /* DiagArray276 */ diag::warn_conditional_mixed_enum_types, -1,
  /* DiagArray277 */ diag::warn_comparison_of_mixed_enum_types_switch, -1,
  /* DiagArray278 */ diag::warn_constexpr_unscoped_enum_out_of_range, -1,
  /* DiagArray279 */ diag::warn_impcast_different_enum_types, -1,
  /* DiagArray280 */ diag::warn_arith_conv_mixed_enum_types, -1,
  /* DiagArray281 */ diag::warn_arith_conv_enum_float, -1,
  /* DiagArray282 */ diag::ext_enum_too_large, diag::ext_enumerator_increment_too_large, -1,
  /* DiagArray283 */ diag::warn_cdtor_function_try_handler_mem_expr, diag::warn_exception_caught_by_earlier_handler, diag::warn_throw_in_noexcept_func, -1,
  /* DiagArray284 */ diag::ext_excess_initializers, diag::ext_excess_initializers_for_sizeless_type, diag::ext_excess_initializers_in_char_array_initializer, diag::ext_initializer_string_for_char_array_too_long, -1,
  /* DiagArray285 */ diag::warn_exit_time_destructor, -1,
  /* DiagArray286 */ diag::warn_defined_in_function_type_macro, diag::warn_defined_in_object_type_macro, -1,
  /* DiagArray287 */ diag::warn_direct_initialize_call, diag::warn_direct_super_initialize_call, -1,
  /* DiagArray288 */ diag::warn_arc_strong_pointer_objc_pointer, -1,
  /* DiagArray289 */ diag::ext_export_no_name_block, diag::ext_export_no_names, -1,
  /* DiagArray290 */ diag::ext_export_using_directive, -1,
  /* DiagArray291 */ diag::warn_zero_size_struct_union_in_extern_c, -1,
  /* DiagArray292 */ diag::warn_extern_init, -1,
  /* DiagArray293 */ diag::warn_arm_interrupt_calling_convention, -1,
  /* DiagArray294 */ diag::warn_namespace_member_extra_qualification, -1,
  /* DiagArray295 */ diag::ext_extra_semi, diag::warn_extra_semi_after_mem_fn_def, -1,
  /* DiagArray296 */ diag::warn_null_statement, -1,
  /* DiagArray297 */ diag::ext_pp_extra_tokens_at_eol, diag::warn_omp_extra_tokens_at_eol, -1,
  /* DiagArray298 */ diag::warn_final_dtor_non_final_class, -1,
  /* DiagArray299 */ diag::warn_pragma_final_macro, -1,
  /* DiagArray300 */ diag::ext_clang_c_enum_fixed_underlying_type, -1,
  /* DiagArray301 */ diag::warn_fixedpoint_constant_overflow, -1,
  /* DiagArray302 */ diag::warn_flag_enum_constant_out_of_range, -1,
  /* DiagArray303 */ diag::ext_flexible_array_in_array, diag::ext_flexible_array_in_struct, -1,
  /* DiagArray304 */ diag::warn_impcast_float_integer, -1,
  /* DiagArray305 */ diag::warn_floatingpoint_eq, -1,
  /* DiagArray306 */ diag::warn_impcast_float_to_integer, diag::warn_impcast_float_to_integer_out_of_range, -1,
  /* DiagArray307 */ diag::warn_impcast_float_to_integer_zero, -1,
  /* DiagArray308 */ diag::warn_redundant_loop_iteration, diag::warn_variables_not_in_loop_body, -1,
  /* DiagArray309 */ diag::warn_format_P_no_precision, diag::warn_format_argument_needs_cast, diag::warn_format_bool_as_character, diag::warn_format_conversion_argument_type_mismatch, diag::warn_format_invalid_annotation, diag::warn_format_invalid_positional_specifier, diag::warn_format_mix_positional_nonpositional_args, diag::warn_format_nonsensical_length, diag::warn_format_string_is_wide_literal, diag::warn_format_zero_positional_specifier, diag::warn_missing_format_string, diag::warn_printf_ObjCflags_without_ObjCConversion, diag::warn_printf_asterisk_missing_arg, diag::warn_printf_asterisk_wrong_type, diag::warn_printf_empty_objc_flag, diag::warn_printf_format_string_contains_null_char, diag::warn_printf_format_string_not_null_terminated, diag::warn_printf_ignored_flag, diag::warn_printf_incomplete_specifier, diag::warn_printf_invalid_objc_flag, diag::warn_printf_narg_not_supported, diag::warn_printf_nonsensical_flag, diag::warn_printf_nonsensical_optional_amount, diag::warn_printf_positional_arg_exceeds_data_args, diag::warn_scanf_nonzero_width, diag::warn_scanf_scanlist_incomplete, -1,
  /* DiagArray310 */ diag::warn_printf_data_arg_not_used, -1,
  /* DiagArray311 */ diag::warn_printf_insufficient_data_args, -1,
  /* DiagArray312 */ diag::warn_format_invalid_conversion, -1,
  /* DiagArray313 */ diag::warn_format_non_standard, diag::warn_format_non_standard_conversion_spec, diag::warn_format_non_standard_positional_arg, -1,
  /* DiagArray314 */ diag::warn_format_nonliteral, -1,
  /* DiagArray315 */ diag::warn_format_argument_needs_cast_pedantic, diag::warn_format_conversion_argument_type_mismatch_pedantic, -1,
  /* DiagArray316 */ diag::warn_format_nonliteral_noargs, -1,
  /* DiagArray317 */ diag::warn_format_conversion_argument_type_mismatch_confusion, -1,
  /* DiagArray319 */ diag::warn_empty_format_string, -1,
  /* DiagArray321 */ diag::warn_fortify_scanf_overflow, diag::warn_fortify_source_format_overflow, diag::warn_fortify_source_overflow, diag::warn_fortify_source_size_mismatch, diag::warn_fortify_strlen_overflow, -1,
  /* DiagArray322 */ diag::warn_four_char_character_literal, -1,
  /* DiagArray323 */ diag::warn_frame_address, -1,
  /* DiagArray324 */ diag::warn_fe_backend_frame_larger_than, diag::warn_fe_frame_larger_than, -1,
  /* DiagArray326 */ diag::warn_framework_include_private_from_public, -1,
  /* DiagArray327 */ diag::warn_free_nonheap_object, -1,
  /* DiagArray328 */ diag::warn_function_def_in_objc_container, -1,
  /* DiagArray329 */ diag::warn_dispatch_body_ignored, diag::warn_multiversion_duplicate_entries, diag::warn_target_clone_duplicate_options, diag::warn_target_clone_no_impact_options, -1,
  /* DiagArray330 */ diag::warn_drv_fuse_ld_path, -1,
  /* DiagArray333 */ diag::ext_clang_diagnose_if, diag::ext_clang_enable_if, diag::ext_warn_gnu_final, diag::warn_attribute_on_function_definition, diag::warn_break_binds_to_switch, diag::warn_cleanup_ext, diag::warn_gcc_attribute_location, diag::warn_gcc_ignores_type_attr, diag::warn_gcc_requires_variadic_function, diag::warn_gcc_variable_decl_in_for_loop, diag::warn_loop_ctrl_binds_to_inner, -1,
  /* DiagArray334 */ diag::warn_global_constructor, diag::warn_global_destructor, -1,
  /* DiagArray335 */ diag::warn_drv_global_isel_incomplete, diag::warn_drv_global_isel_incomplete_opt, -1,
  /* DiagArray337 */ diag::ext_alignof_expr, -1,
  /* DiagArray338 */ diag::ext_gnu_anonymous_struct, -1,
  /* DiagArray339 */ diag::ext_array_init_parens, -1,
  /* DiagArray340 */ diag::ext_auto_type, -1,
  /* DiagArray341 */ diag::ext_binary_literal, -1,
  /* DiagArray342 */ diag::ext_gnu_case_range, -1,
  /* DiagArray343 */ diag::ext_integer_complex, -1,
  /* DiagArray344 */ diag::ext_array_init_copy, -1,
  /* DiagArray345 */ diag::ext_gnu_conditional_expr, -1,
  /* DiagArray346 */ diag::ext_gnu_array_range, diag::ext_gnu_missing_equal_designator, diag::ext_gnu_old_style_field_designator, -1,
  /* DiagArray347 */ diag::ext_gnu_empty_initializer, -1,
  /* DiagArray348 */ diag::ext_empty_struct_union, diag::ext_flexible_array_empty_aggregate_gnu, diag::ext_no_named_members_in_struct_union, -1,
  /* DiagArray349 */ diag::ext_flexible_array_init, -1,
  /* DiagArray350 */ diag::ext_flexible_array_union_gnu, -1,
  /* DiagArray351 */ diag::ext_expr_not_ice, diag::ext_in_class_initializer_non_constant, diag::ext_vla_folded_to_constant, -1,
  /* DiagArray352 */ diag::ext_imaginary_constant, -1,
  /* DiagArray353 */ diag::ext_pp_include_next_directive, -1,
  /* DiagArray354 */ diag::warn_gnu_inline_cplusplus_without_extern, -1,
  /* DiagArray355 */ diag::ext_gnu_address_of_label, diag::ext_gnu_indirect_goto, -1,
  /* DiagArray356 */ diag::ext_pp_gnu_line_directive, -1,
  /* DiagArray357 */ diag::warn_gnu_null_ptr_arith, -1,
  /* DiagArray358 */ diag::ext_gnu_ptr_func_arith, diag::ext_gnu_subscript_void_type, diag::ext_gnu_void_ptr, -1,
  /* DiagArray359 */ diag::ext_forward_ref_enum_def, -1,
  /* DiagArray360 */ diag::ext_gnu_statement_expr, -1,
  /* DiagArray361 */ diag::ext_gnu_statement_expr_macro, -1,
  /* DiagArray362 */ diag::ext_in_class_initializer_float_type, -1,
  /* DiagArray363 */ diag::ext_string_literal_operator_template, -1,
  /* DiagArray364 */ diag::ext_typecheck_cast_to_union, -1,
  /* DiagArray365 */ diag::ext_variable_sized_type_in_struct, -1,
  /* DiagArray366 */ diag::ext_pp_line_zero, -1,
  /* DiagArray367 */ diag::ext_missing_varargs_arg, diag::ext_paste_comma, -1,
  /* DiagArray368 */ diag::warn_maybe_capture_bad_target_this_ptr, -1,
  /* DiagArray369 */ diag::warn_header_guard, -1,
  /* DiagArray370 */ diag::warn_using_directive_in_header, -1,
  /* DiagArray371 */ diag::warn_ignored_hip_only_option, -1,
  /* DiagArray372 */ diag::ext_hlsl_access_specifiers, -1,
  /* DiagArray373 */ diag::warn_condition_is_idiomatic_assignment, -1,
  /* DiagArray374 */ diag::ext_cannot_use_trivial_abi, diag::warn_alias_to_weak_alias, diag::warn_alias_with_section, diag::warn_aligned_attr_underaligned, diag::warn_attr_abi_tag_namespace, diag::warn_attribute_after_definition_ignored, diag::warn_attribute_cmse_entry_static, diag::warn_attribute_dllexport_explicit_instantiation_def, diag::warn_attribute_has_no_effect_on_compile_time_if, diag::warn_attribute_has_no_effect_on_infinite_loop, diag::warn_attribute_iboutlet, diag::warn_attribute_ignored, diag::warn_attribute_ignored_for_field_of_type, diag::warn_attribute_ignored_no_calls_in_stmt, diag::warn_attribute_ignored_on_inline, diag::warn_attribute_ignored_on_non_definition, diag::warn_attribute_invalid_on_definition, diag::warn_attribute_no_decl, diag::warn_attribute_nonnull_no_pointers, diag::warn_attribute_nonnull_parm_no_args, diag::warn_attribute_not_on_decl, diag::warn_attribute_pointer_or_reference_only, diag::warn_attribute_pointers_only, diag::warn_attribute_precede_definition, diag::warn_attribute_return_pointers_only, diag::warn_attribute_return_pointers_refs_only, diag::warn_attribute_sentinel_named_arguments, diag::warn_attribute_sentinel_not_variadic, diag::warn_attribute_type_not_supported, diag::warn_attribute_type_not_supported_global, diag::warn_attribute_unknown_visibility, diag::warn_attribute_void_function_method, diag::warn_attribute_weak_on_field, diag::warn_attribute_weak_on_local, diag::warn_attribute_wrong_decl_type, diag::warn_attribute_wrong_decl_type_str, diag::warn_attributes_likelihood_ifstmt_conflict, diag::warn_block_literal_attributes_on_omitted_return_type, diag::warn_cconv_unsupported, diag::warn_cxx11_gnu_attribute_on_type, diag::warn_declspec_allocator_nonpointer, diag::warn_declspec_attribute_ignored, diag::warn_deprecated_anonymous_namespace, diag::warn_deprecated_ignored_on_using, diag::warn_dllimport_dropped_from_inline_function, diag::warn_duplicate_attribute, diag::warn_duplicate_attribute_exact, diag::warn_function_attribute_ignored_in_stmt, diag::warn_function_stmt_attribute_precedence, diag::warn_gc_attribute_weak_on_local, diag::warn_gnu_inline_attribute_requires_inline, diag::warn_ignored_ms_inheritance, diag::warn_ignored_objc_externally_retained, diag::warn_import_on_definition, diag::warn_internal_linkage_local_storage, diag::warn_interrupt_attribute_invalid, diag::warn_microsoft_qualifiers_ignored, diag::warn_mig_server_routine_does_not_return_kern_return_t, diag::warn_mismatched_import, diag::warn_mmap_unknown_attribute, diag::warn_nocf_check_attribute_ignored, diag::warn_noderef_on_non_pointer_or_array, diag::warn_nothrow_attribute_ignored, diag::warn_ns_attribute_wrong_parameter_type, diag::warn_ns_attribute_wrong_return_type, diag::warn_objc_direct_ignored, diag::warn_objc_direct_property_ignored, diag::warn_opencl_attr_deprecated_ignored, diag::warn_require_const_init_added_too_late, diag::warn_riscv_repeated_interrupt_attribute, diag::warn_sycl_kernel_invalid_template_param_type, diag::warn_sycl_kernel_num_of_function_params, diag::warn_sycl_kernel_num_of_template_params, diag::warn_sycl_kernel_return_type, diag::warn_transparent_union_attribute_field_size_align, diag::warn_transparent_union_attribute_floating, diag::warn_transparent_union_attribute_not_definition, diag::warn_transparent_union_attribute_zero_fields, diag::warn_type_attribute_wrong_type, diag::warn_unhandled_ms_attribute_ignored, diag::warn_unsupported_target_attribute, diag::warn_unused_result_typedef_unsupported_spelling, diag::warn_wrong_clang_attr_namespace, -1,
  /* DiagArray375 */ diag::warn_missing_sdksettings_for_availability_checking, -1,
  /* DiagArray376 */ diag::warn_drv_unsupported_opt_for_target, diag::warn_ignored_gcc_optimization, -1,
  /* DiagArray377 */ diag::warn_pragma_intrinsic_builtin, -1,
  /* DiagArray379 */ diag::warn_clause_expected_string, diag::warn_pragma_align_expected_equal, diag::warn_pragma_align_invalid_option, diag::warn_pragma_comment_ignored, diag::warn_pragma_debug_missing_argument, diag::warn_pragma_debug_missing_command, diag::warn_pragma_debug_unexpected_argument, diag::warn_pragma_debug_unexpected_command, diag::warn_pragma_debug_unknown_module, diag::warn_pragma_expected_action_or_r_paren, diag::warn_pragma_expected_colon, diag::warn_pragma_expected_colon_r_paren, diag::warn_pragma_expected_comma, diag::warn_pragma_expected_identifier, diag::warn_pragma_expected_init_seg, diag::warn_pragma_expected_integer, diag::warn_pragma_expected_lparen, diag::warn_pragma_expected_non_wide_string, diag::warn_pragma_expected_predicate, diag::warn_pragma_expected_punc, diag::warn_pragma_expected_rparen, diag::warn_pragma_expected_section_label_or_name, diag::warn_pragma_expected_section_name, diag::warn_pragma_expected_section_push_pop_or_name, diag::warn_pragma_expected_string, diag::warn_pragma_extra_tokens_at_eol, diag::warn_pragma_force_cuda_host_device_bad_arg, diag::warn_pragma_fp_ignored, diag::warn_pragma_init_seg_unsupported_target, diag::warn_pragma_invalid_action, diag::warn_pragma_invalid_argument, diag::warn_pragma_invalid_specific_action, diag::warn_pragma_missing_argument, diag::warn_pragma_ms_fenv_access, diag::warn_pragma_ms_struct, diag::warn_pragma_options_align_reset_failed, diag::warn_pragma_options_expected_align, diag::warn_pragma_pack_invalid_alignment, diag::warn_pragma_pack_malformed, diag::warn_pragma_pop_failed, diag::warn_pragma_pop_macro_no_push, diag::warn_pragma_unknown_extension, diag::warn_pragma_unsupported_action, diag::warn_pragma_unsupported_extension, diag::warn_pragma_unused_expected_var, diag::warn_pragma_unused_expected_var_arg, diag::warn_pragma_unused_undeclared_var, diag::warn_stdc_unknown_rounding_mode, -1,
  /* DiagArray380 */ diag::warn_arc_lifetime_result_type, diag::warn_block_literal_qualifiers_on_omitted_return_type, diag::warn_qual_return_type, diag::warn_typecheck_function_qualifiers_ignored, -1,
  /* DiagArray381 */ diag::warn_typecheck_reference_qualifiers, -1,
  /* DiagArray383 */ diag::warn_auto_implicit_atomic_property, diag::warn_implicit_atomic_property, -1,
  /* DiagArray384 */ diag::warn_impcast_integer_float_precision_constant, -1,
  /* DiagArray385 */ diag::warn_impcast_floating_point_to_bool, -1,
  /* DiagArray386 */ diag::ext_implicit_exception_spec_mismatch, -1,
  /* DiagArray387 */ diag::warn_unannotated_fallthrough, -1,
  /* DiagArray388 */ diag::warn_unannotated_fallthrough_per_function, -1,
  /* DiagArray389 */ diag::warn_impcast_fixed_point_range, -1,
  /* DiagArray390 */ diag::warn_impcast_float_precision, diag::warn_impcast_float_result_precision, -1,
  /* DiagArray391 */ diag::ext_implicit_function_decl_c99, diag::ext_implicit_lib_function_decl, diag::ext_implicit_lib_function_decl_c99, diag::warn_builtin_unknown, diag::warn_implicit_function_decl, -1,
  /* DiagArray392 */ diag::ext_missing_type_specifier, diag::ext_param_not_declared, diag::warn_missing_type_specifier, -1,
  /* DiagArray393 */ diag::warn_impcast_high_order_zero_bits, diag::warn_impcast_integer_precision, -1,
  /* DiagArray394 */ diag::warn_impcast_integer_float_precision, -1,
  /* DiagArray395 */ diag::warn_implicitly_retains_self, -1,
  /* DiagArray396 */ diag::ext_integer_literal_too_large_for_signed, -1,
  /* DiagArray398 */ diag::ext_pp_import_directive, -1,
  /* DiagArray399 */ diag::warn_inaccessible_base_class, -1,
  /* DiagArray400 */ diag::pp_include_next_absolute_path, -1,
  /* DiagArray401 */ diag::pp_include_next_in_primary, -1,
  /* DiagArray402 */ diag::warn_deep_exception_specs_differ, diag::warn_incompatible_exception_specs, -1,
  /* DiagArray403 */ diag::ext_typecheck_convert_incompatible_function_pointer, -1,
  /* DiagArray404 */ diag::warn_typecheck_convert_incompatible_function_pointer_strict, -1,
  /* DiagArray405 */ diag::warn_redecl_library_builtin, -1,
  /* DiagArray406 */ diag::warn_cxx_ms_struct, diag::warn_npot_ms_struct, -1,
  /* DiagArray407 */ diag::ext_typecheck_convert_incompatible_pointer, -1,
  /* DiagArray408 */ diag::ext_nested_pointer_qualifier_mismatch, diag::ext_typecheck_convert_discards_qualifiers, diag::warn_bad_cxx_cast_nested_pointer_addr_space, -1,
  /* DiagArray409 */ diag::warn_property_types_are_incompatible, -1,
  /* DiagArray410 */ diag::warn_incompatible_sysroot, -1,
  /* DiagArray411 */ diag::warn_mmap_incomplete_framework_module_declaration, -1,
  /* DiagArray412 */ diag::warn_undef_method_impl, -1,
  /* DiagArray414 */ diag::warn_implicit_decl_no_jmp_buf, -1,
  /* DiagArray415 */ diag::warn_missing_submodule, diag::warn_mmap_umbrella_dir_not_found, diag::warn_uncovered_module_header, -1,
  /* DiagArray416 */ diag::warn_redeclaration_without_attribute_prev_attribute_ignored, diag::warn_redeclaration_without_import_attribute, -1,
  /* DiagArray417 */ diag::warn_inconsistent_destructor_marked_not_override_overriding, -1,
  /* DiagArray418 */ diag::warn_inconsistent_function_marked_not_override_overriding, -1,
  /* DiagArray419 */ diag::ext_increment_bool, -1,
  /* DiagArray420 */ diag::warn_infinite_recursive_function, -1,
  /* DiagArray422 */ diag::ext_initializer_overrides, diag::warn_initializer_overrides, -1,
  /* DiagArray423 */ diag::ext_out_of_line_qualified_id_type_names_constructor, -1,
  /* DiagArray425 */ diag::warn_fe_inline_asm, -1,
  /* DiagArray426 */ diag::warn_inline_namespace_reopened_noninline, -1,
  /* DiagArray427 */ diag::ext_operator_new_delete_declared_inline, -1,
  /* DiagArray428 */ diag::warn_explicit_instantiation_after_specialization, -1,
  /* DiagArray429 */ diag::ext_typecheck_convert_int_pointer, diag::ext_typecheck_convert_pointer_int, -1,
  /* DiagArray431 */ diag::warn_enum_constant_in_bool_context, diag::warn_left_shift_in_bool_context, -1,
  /* DiagArray432 */ diag::warn_int_to_pointer_cast, -1,
  /* DiagArray433 */ diag::warn_int_to_void_pointer_cast, -1,
  /* DiagArray434 */ diag::warn_integer_constant_overflow, -1,
  /* DiagArray435 */ diag::warn_anyx86_interrupt_regsave, -1,
  /* DiagArray436 */ diag::warn_drv_missing_plugin_arg, diag::warn_drv_missing_plugin_name, diag::warn_drv_msp430_hwmult_mismatch, diag::warn_drv_msp430_hwmult_no_device, diag::warn_drv_msp430_hwmult_unsupported, diag::warn_drv_object_size_disabled_O0, diag::warn_drv_optimization_value, diag::warn_fe_backend_invalid_feature_flag, diag::warn_target_unsupported_extension, -1,
  /* DiagArray437 */ diag::ext_constexpr_function_never_constant_expr, -1,
  /* DiagArray438 */ diag::warn_iboutlet_object_type, diag::warn_iboutletcollection_property_assign, -1,
  /* DiagArray439 */ diag::warn_invalid_initializer_from_system_header, -1,
  /* DiagArray440 */ diag::warn_invalid_ios_deployment_target, -1,
  /* DiagArray441 */ diag::warn_attribute_no_builtin_invalid_builtin_name, -1,
  /* DiagArray442 */ diag::warn_falloff_noreturn_function, diag::warn_noreturn_function_has_return_expr, -1,
  /* DiagArray443 */ diag::ext_offsetof_non_pod_type, diag::ext_offsetof_non_standardlayout_type, -1,
  /* DiagArray444 */ diag::warn_drv_ps_sdk_dir, diag::warn_drv_unable_to_find_directory_expected, -1,
  /* DiagArray445 */ diag::ext_partial_spec_not_more_specialized_than_primary, -1,
  /* DiagArray447 */ diag::ext_empty_character, diag::ext_unterminated_char_or_string, -1,
  /* DiagArray448 */ diag::warn_bad_character_encoding, diag::warn_bad_string_encoding, -1,
  /* DiagArray449 */ diag::ext_pp_bad_paste_ms, -1,
  /* DiagArray450 */ diag::warn_invalid_utf8_in_comment, -1,
  /* DiagArray451 */ diag::warn_jump_out_of_seh_finally, -1,
  /* DiagArray452 */ diag::ext_keyword_as_ident, -1,
  /* DiagArray453 */ diag::warn_pp_macro_hides_keyword, -1,
  /* DiagArray454 */ diag::ext_param_promoted_not_compatible_with_prototype, -1,
  /* DiagArray455 */ diag::ext_token_used, -1,
  /* DiagArray456 */ diag::warn_parameter_size, diag::warn_return_value_size, -1,
  /* DiagArray458 */ diag::warn_fe_linking_module, -1,
  /* DiagArray459 */ diag::warn_impcast_literal_float_to_integer, diag::warn_impcast_literal_float_to_integer_out_of_range, -1,
  /* DiagArray460 */ diag::warn_float_compare_literal, diag::warn_float_overflow, diag::warn_float_underflow, -1,
  /* DiagArray461 */ diag::ext_template_arg_local_type, -1,
  /* DiagArray462 */ diag::warn_logical_not_on_lhs_of_check, -1,
  /* DiagArray463 */ diag::warn_logical_and_in_logical_or, -1,
  /* DiagArray464 */ diag::ext_c99_longlong, -1,
  /* DiagArray466 */ diag::ext_pp_macro_redef, -1,
  /* DiagArray467 */ diag::ext_main_used, diag::ext_noreturn_main, diag::ext_variadic_main, diag::warn_main_one_arg, diag::warn_main_redefined, diag::warn_main_returns_bool_literal, diag::warn_static_main, -1,
  /* DiagArray468 */ diag::ext_main_returns_nonint, -1,
  /* DiagArray469 */ diag::warn_has_warning_invalid_option, -1,
  /* DiagArray470 */ diag::ext_many_braces_around_init, -1,
  /* DiagArray471 */ diag::ext_mathematical_notation, -1,
  /* DiagArray472 */ diag::warn_max_tokens, diag::warn_max_tokens_total, -1,
  /* DiagArray473 */ diag::warn_max_unsigned_zero, -1,
  /* DiagArray474 */ diag::warn_suspicious_sizeof_memset, -1,
  /* DiagArray475 */ diag::warn_memsize_comparison, -1,
  /* DiagArray476 */ diag::warn_non_contravariant_param_types, diag::warn_non_covariant_ret_types, -1,
  /* DiagArray478 */ diag::ext_ms_abstract_keyword, -1,
  /* DiagArray479 */ diag::ext_anonymous_record_with_type, diag::ext_ms_anonymous_record, -1,
  /* DiagArray480 */ diag::ext_ms_cast_fn_obj, diag::ext_ms_impcast_fn_obj, -1,
  /* DiagArray481 */ diag::ext_charize_microsoft, -1,
  /* DiagArray482 */ diag::ext_comment_paste_microsoft, -1,
  /* DiagArray483 */ diag::ext_default_init_const, -1,
  /* DiagArray484 */ diag::ext_pp_operator_used_as_macro_name, -1,
  /* DiagArray485 */ diag::ext_param_default_argument_redefinition, -1,
  /* DiagArray486 */ diag::warn_attribute_section_drectve, -1,
  /* DiagArray487 */ diag::ext_ctrl_z_eof_microsoft, -1,
  /* DiagArray488 */ diag::ext_ms_forward_ref_enum, -1,
  /* DiagArray489 */ diag::ext_enumerator_too_large, -1,
  /* DiagArray490 */ diag::ext_ellipsis_exception_spec, diag::ext_incomplete_in_exception_spec, diag::ext_mismatched_exception_spec, diag::ext_mismatched_exception_spec_explicit_instantiation, diag::ext_override_exception_spec, -1,
  /* DiagArray491 */ diag::warn_microsoft_dependent_exists, -1,
  /* DiagArray492 */ diag::ext_ms_explicit_constructor_call, -1,
  /* DiagArray493 */ diag::warn_member_extra_qualification, -1,
  /* DiagArray494 */ diag::ext_ms_c_enum_fixed_underlying_type, -1,
  /* DiagArray495 */ diag::ext_flexible_array_empty_aggregate_ms, diag::ext_flexible_array_union_ms, -1,
  /* DiagArray496 */ diag::ext_goto_into_protected_scope, -1,
  /* DiagArray497 */ diag::ext_ms_ambiguous_direct_base, -1,
  /* DiagArray498 */ diag::ext_pp_include_search_ms, -1,
  /* DiagArray499 */ diag::ext_mutable_reference, -1,
  /* DiagArray500 */ diag::ext_pure_function_definition, -1,
  /* DiagArray501 */ diag::ext_static_non_static, -1,
  /* DiagArray502 */ diag::ext_ms_sealed_keyword, -1,
  /* DiagArray503 */ diag::ext_ms_static_assert, -1,
  /* DiagArray504 */ diag::ext_explicit_instantiation_duplicate, diag::ext_found_in_dependent_base, diag::ext_found_later_in_class, diag::ext_ms_delayed_template_argument, diag::ext_ms_deref_template_argument, diag::ext_ms_template_spec_redecl_out_of_scope, diag::ext_ms_template_type_arg_missing_typename, diag::ext_static_out_of_line, diag::ext_undeclared_unqual_id_with_dependent_base, diag::ext_unqualified_base_class, -1,
  /* DiagArray505 */ diag::ext_template_param_shadow, -1,
  /* DiagArray506 */ diag::ext_union_member_of_reference_type, -1,
  /* DiagArray507 */ diag::ext_friend_tag_redecl_outside_namespace, -1,
  /* DiagArray508 */ diag::ext_ms_using_declaration_inaccessible, -1,
  /* DiagArray509 */ diag::ext_pseudo_dtor_on_void, -1,
  /* DiagArray510 */ diag::warn_profile_data_misexpect, -1,
  /* DiagArray511 */ diag::warn_misleading_indentation, -1,
  /* DiagArray512 */ diag::warn_mismatched_delete_new, -1,
  /* DiagArray513 */ diag::warn_conflicting_param_types, -1,
  /* DiagArray514 */ diag::warn_conflicting_ret_types, -1,
  /* DiagArray515 */ diag::warn_struct_class_previous_tag_mismatch, diag::warn_struct_class_tag_mismatch, -1,
  /* DiagArray516 */ diag::warn_missing_braces, -1,
  /* DiagArray517 */ diag::ext_constinit_missing, -1,
  /* DiagArray518 */ diag::ext_no_declarators, diag::ext_standalone_specifier, diag::ext_typedef_without_a_name, diag::warn_standalone_specifier, -1,
  /* DiagArray519 */ diag::ext_missing_exception_specification, -1,
  /* DiagArray520 */ diag::warn_missing_field_initializers, -1,
  /* DiagArray523 */ diag::warn_missing_method_return_type, -1,
  /* DiagArray524 */ diag::warn_overriding_method_missing_noescape, -1,
  /* DiagArray525 */ diag::warn_suggest_noreturn_block, diag::warn_suggest_noreturn_function, -1,
  /* DiagArray526 */ diag::warn_cconv_knr, -1,
  /* DiagArray527 */ diag::warn_missing_prototype, -1,
  /* DiagArray528 */ diag::warn_missing_selector_name, -1,
  /* DiagArray529 */ diag::warn_missing_sysroot, -1,
  /* DiagArray530 */ diag::warn_missing_variable_declarations, -1,
  /* DiagArray531 */ diag::warn_assume_attribute_string_unknown_suggested, -1,
  /* DiagArray532 */ diag::remark_module_build, diag::remark_module_build_done, diag::remark_module_lock_failure, diag::remark_module_lock_timeout, -1,
  /* DiagArray533 */ diag::warn_module_conflict, diag::warn_module_system_bit_conflict, -1,
  /* DiagArray534 */ diag::warn_module_config_mismatch, -1,
  /* DiagArray535 */ diag::warn_duplicate_module_file_extension, -1,
  /* DiagArray536 */ diag::remark_module_import, -1,
  /* DiagArray537 */ diag::ext_module_import_in_extern_c, -1,
  /* DiagArray538 */ diag::remark_pp_include_directive_modular_translation, -1,
  /* DiagArray539 */ diag::remark_module_lock, -1,
  /* DiagArray540 */ diag::ext_equivalent_internal_linkage_decl_in_modules, -1,
  /* DiagArray541 */ diag::ext_module_import_not_at_top_level_noop, -1,
  /* DiagArray545 */ diag::warn_drv_msvc_not_found, -1,
  /* DiagArray546 */ diag::warn_multichar_character_literal, -1,
  /* DiagArray547 */ diag::warn_vbase_moved_multiple_times, -1,
  /* DiagArray549 */ diag::ext_anonymous_record_with_anonymous_type, -1,
  /* DiagArray551 */ diag::warn_operator_new_returns_null, -1,
  /* DiagArray552 */ diag::ext_no_newline_eof, diag::warn_no_newline_eof, -1,
  /* DiagArray553 */ diag::warn_dereference_of_noderef_type, diag::warn_dereference_of_noderef_type_no_decl, diag::warn_noderef_to_dereferenceable_pointer, -1,
  /* DiagArray555 */ diag::ext_non_c_like_anon_struct_in_typedef, -1,
  /* DiagArray557 */ diag::warn_non_literal_null_pointer, -1,
  /* DiagArray558 */ diag::warn_non_modular_include_in_framework_module, -1,
  /* DiagArray559 */ diag::warn_non_modular_include_in_module, -1,
  /* DiagArray560 */ diag::warn_cannot_pass_non_pod_arg_to_vararg, diag::warn_non_pod_vararg_with_format_string, diag::warn_second_parameter_to_va_arg_not_pod, diag::warn_second_parameter_to_va_arg_ownership_qualified, -1,
  /* DiagArray561 */ diag::warn_alignment_not_power_of_two, -1,
  /* DiagArray562 */ diag::warn_non_virtual_dtor, -1,
  /* DiagArray563 */ diag::warn_null_arg, diag::warn_null_ret, -1,
  /* DiagArray565 */ diag::pp_nonportable_path, -1,
  /* DiagArray566 */ diag::pp_nonportable_system_path, -1,
  /* DiagArray567 */ diag::warn_neon_vector_initializer_non_portable, -1,
  /* DiagArray568 */ diag::warn_cstruct_memaccess, -1,
  /* DiagArray569 */ diag::warn_nsconsumed_attribute_mismatch, -1,
  /* DiagArray570 */ diag::warn_nsreturns_retained_attribute_mismatch, -1,
  /* DiagArray571 */ diag::warn_null_in_arithmetic_operation, diag::warn_null_in_comparison_operation, -1,
  /* DiagArray572 */ diag::null_in_char_or_string, diag::null_in_file, -1,
  /* DiagArray573 */ diag::warn_impcast_null_pointer_to_integer, -1,
  /* DiagArray574 */ diag::warn_binding_null_to_reference, diag::warn_indirection_through_null, -1,
  /* DiagArray575 */ diag::warn_pointer_arith_null_ptr, -1,
  /* DiagArray576 */ diag::warn_pointer_sub_null_ptr, -1,
  /* DiagArray577 */ diag::warn_conflicting_nullability_attr_overriding_param_types, diag::warn_conflicting_nullability_attr_overriding_ret_types, diag::warn_mismatched_nullability_attr, diag::warn_null_resettable_setter, diag::warn_nullability_duplicate, -1,
  /* DiagArray578 */ diag::warn_nullability_missing, -1,
  /* DiagArray579 */ diag::warn_nullability_missing_array, -1,
  /* DiagArray580 */ diag::warn_nullability_declspec, -1,
  /* DiagArray581 */ diag::ext_nullability, -1,
  /* DiagArray582 */ diag::warn_nullability_inferred_on_nested_type, -1,
  /* DiagArray583 */ diag::warn_nullability_lost, -1,
  /* DiagArray584 */ diag::warn_autosynthesis_property_ivar_match, -1,
  /* DiagArray585 */ diag::warn_impcast_constant_value_to_objc_bool, -1,
  /* DiagArray586 */ diag::warn_objc_boxing_invalid_utf8_string, -1,
  /* DiagArray587 */ diag::warn_objc_circular_container, -1,
  /* DiagArray589 */ diag::warn_objc_designated_init_missing_super_call, diag::warn_objc_designated_init_non_designated_init_call, diag::warn_objc_designated_init_non_super_designated_init_call, diag::warn_objc_implementation_missing_designated_init_override, diag::warn_objc_secondary_init_missing_init_call, diag::warn_objc_secondary_init_super_init_call, -1,
  /* DiagArray590 */ diag::warn_nsdictionary_duplicate_key, -1,
  /* DiagArray591 */ diag::warn_superclass_variable_sized_type_not_at_end, diag::warn_variable_sized_ivar_visibility, -1,
  /* DiagArray592 */ diag::warn_forward_class_redefinition, -1,
  /* DiagArray593 */ diag::warn_ivars_in_interface, -1,
  /* DiagArray594 */ diag::warn_objc_literal_comparison, -1,
  /* DiagArray595 */ diag::warn_impcast_objective_c_literal_to_bool, diag::warn_objc_collection_literal_element, -1,
  /* DiagArray596 */ diag::warn_pp_objc_macro_redef_ignored, -1,
  /* DiagArray597 */ diag::warn_messaging_unqualified_id, -1,
  /* DiagArray598 */ diag::warn_class_method_not_found, diag::warn_class_method_not_found_with_typo, diag::warn_inst_method_not_found, diag::warn_instance_method_not_found_with_typo, diag::warn_instance_method_on_class_found, diag::warn_root_inst_method_not_found, -1,
  /* DiagArray599 */ diag::warn_missing_explicit_synthesis, -1,
  /* DiagArray600 */ diag::warn_objc_missing_super_call, -1,
  /* DiagArray601 */ diag::warn_multiple_method_decl, -1,
  /* DiagArray602 */ diag::warn_objc_property_retain_of_block, -1,
  /* DiagArray603 */ diag::warn_objc_pointer_cxx_catch_fragile, -1,
  /* DiagArray604 */ diag::warn_objc_property_assign_on_object, -1,
  /* DiagArray605 */ diag::warn_impl_required_for_class_property, diag::warn_impl_required_in_category_for_class_property, diag::warn_setter_getter_impl_required, diag::warn_setter_getter_impl_required_in_category, -1,
  /* DiagArray606 */ diag::warn_property_implicitly_mismatched, -1,
  /* DiagArray607 */ diag::warn_cocoa_naming_owned_rule, -1,
  /* DiagArray608 */ diag::warn_objc_property_default_assign_on_object, diag::warn_objc_property_no_assignment_attribute, -1,
  /* DiagArray609 */ diag::warn_autosynthesis_property_in_superclass, diag::warn_no_autosynthesis_property, diag::warn_no_autosynthesis_shared_ivar_property, -1,
  /* DiagArray610 */ diag::warn_category_method_impl_match, -1,
  /* DiagArray611 */ diag::warn_auto_synthesizing_protocol_property, -1,
  /* DiagArray612 */ diag::warn_objc_redundant_qualified_class_type, -1,
  /* DiagArray613 */ diag::warn_objc_readonly_property_has_setter, -1,
  /* DiagArray615 */ diag::warn_objc_redundant_literal_use, -1,
  /* DiagArray616 */ diag::warn_objc_root_class_missing, -1,
  /* DiagArray618 */ diag::warn_impcast_float_to_objc_signed_char_bool, -1,
  /* DiagArray619 */ diag::warn_impcast_int_to_objc_signed_char_bool, -1,
  /* DiagArray620 */ diag::warn_objc_string_literal_comparison, -1,
  /* DiagArray621 */ diag::warn_concatenated_nsarray_literal, -1,
  /* DiagArray622 */ diag::warn_objc_unsafe_perform_selector, -1,
  /* DiagArray623 */ diag::warn_odr_different_num_template_parameters, diag::warn_odr_different_template_parameter_kind, diag::warn_odr_field_type_inconsistent, diag::warn_odr_function_type_inconsistent, diag::warn_odr_ivar_type_inconsistent, diag::warn_odr_non_type_parameter_type_inconsistent, diag::warn_odr_objc_method_num_params_inconsistent, diag::warn_odr_objc_method_param_type_inconsistent, diag::warn_odr_objc_method_result_type_inconsistent, diag::warn_odr_objc_method_variadic_inconsistent, diag::warn_odr_objc_property_impl_kind_inconsistent, diag::warn_odr_objc_property_type_inconsistent, diag::warn_odr_objc_superclass_inconsistent, diag::warn_odr_objc_synthesize_ivar_inconsistent, diag::warn_odr_parameter_pack_non_pack, diag::warn_odr_tag_type_inconsistent, diag::warn_odr_variable_multiple_def, diag::warn_odr_variable_type_inconsistent, -1,
  /* DiagArray624 */ diag::warn_old_style_cast, -1,
  /* DiagArray626 */ diag::ext_opencl_ext_vector_type_rgba_selector, -1,
  /* DiagArray628 */ diag::ext_omp_attributes, -1,
  /* DiagArray629 */ diag::warn_omp_alignment_not_power_of_two, diag::warn_omp_allocate_thread_on_task_target_directive, diag::warn_omp_ctx_incompatible_property_for_selector, diag::warn_omp_ctx_incompatible_score_for_property, diag::warn_omp_ctx_incompatible_selector_for_set, diag::warn_omp_ctx_selector_without_properties, diag::warn_omp_declare_variant_ctx_mutiple_use, diag::warn_omp_declare_variant_ctx_not_a_property, diag::warn_omp_declare_variant_ctx_not_a_selector, diag::warn_omp_declare_variant_ctx_not_a_set, diag::warn_omp_declare_variant_expected, diag::warn_omp_declare_variant_string_literal_or_identifier, diag::warn_omp_linear_step_zero, diag::warn_omp_more_one_device_type_clause, diag::warn_omp_more_one_interop_type, diag::warn_omp_more_one_omp_all_memory, diag::warn_omp_unknown_assumption_clause_missing_id, diag::warn_omp_unknown_assumption_clause_without_args, diag::warn_omp_used_different_allocator, -1,
  /* DiagArray630 */ diag::ext_omp_loop_not_canonical_init, diag::warn_omp_loop_64_bit_var, -1,
  /* DiagArray631 */ diag::warn_omp_non_trivial_type_mapped, -1,
  /* DiagArray632 */ diag::warn_drv_omp_offload_target_duplicate, diag::warn_omp_declare_target_after_first_use, diag::warn_omp_not_in_target_context, -1,
  /* DiagArray633 */ diag::warn_drv_fine_grained_bitfield_accesses_ignored, diag::warn_drv_fjmc_for_elf_only, diag::warn_drv_jmc_requires_debuginfo, diag::warn_drv_moutline_atomics_unsupported_opt, diag::warn_drv_moutline_unsupported_opt, diag::warn_drv_needs_hvx, diag::warn_drv_ps_force_pic, diag::warn_drv_unsupported_longcalls, diag::warn_drv_unsupported_option_for_flang, diag::warn_drv_unsupported_option_for_offload_arch_req_feature, diag::warn_drv_unsupported_option_for_target, diag::warn_drv_unsupported_pic_with_mabicalls, diag::warn_drv_unsupported_sdata, diag::warn_target_override_arm64ec, -1,
  /* DiagArray634 */ diag::ext_typecheck_ordered_comparison_of_function_pointers, diag::warn_typecheck_ordered_comparison_of_function_pointers, -1,
  /* DiagArray635 */ diag::ext_out_of_line_declaration, -1,
  /* DiagArray636 */ diag::ext_use_out_of_scope_declaration, -1,
  /* DiagArray637 */ diag::warn_overaligned_type, -1,
  /* DiagArray639 */ diag::ext_string_too_long, -1,
  /* DiagArray640 */ diag::warn_overloaded_shift_in_comparison, -1,
  /* DiagArray641 */ diag::warn_overloaded_virtual, -1,
  /* DiagArray643 */ diag::warn_fe_override_module, -1,
  /* DiagArray644 */ diag::warn_conflicting_overriding_param_modifiers, diag::warn_conflicting_overriding_param_types, diag::warn_conflicting_overriding_ret_type_modifiers, diag::warn_conflicting_overriding_ret_types, diag::warn_conflicting_overriding_variadic, diag::warn_non_contravariant_overriding_param_types, diag::warn_non_covariant_overriding_ret_types, -1,
  /* DiagArray645 */ diag::warn_drv_overriding_flag_option, -1,
  /* DiagArray646 */ diag::warn_unnecessary_packed, -1,
  /* DiagArray647 */ diag::warn_unpacked_field, -1,
  /* DiagArray648 */ diag::warn_padded_struct_anon_field, diag::warn_padded_struct_field, diag::warn_padded_struct_size, -1,
  /* DiagArray649 */ diag::warn_condition_is_assignment, diag::warn_precedence_bitwise_rel, diag::warn_precedence_conditional, -1,
  /* DiagArray650 */ diag::warn_equality_with_extra_parens, -1,
  /* DiagArray652 */ diag::remark_fe_backend_optimization_remark, -1,
  /* DiagArray653 */ diag::remark_fe_backend_optimization_remark_analysis, diag::remark_fe_backend_optimization_remark_analysis_aliasing, diag::remark_fe_backend_optimization_remark_analysis_fpcommute, -1,
  /* DiagArray654 */ diag::warn_fe_backend_optimization_failure, -1,
  /* DiagArray655 */ diag::remark_fe_backend_optimization_remark_missed, -1,
  /* DiagArray656 */ diag::warn_module_uses_date_time, -1,
  /* DiagArray657 */ diag::ext_aggregate_init_not_constant, diag::ext_anonymous_record_with_type, diag::ext_anonymous_struct_union_qualified, diag::ext_array_size_conversion, diag::ext_auto_new_list_init, diag::ext_c99_array_usage, diag::ext_c99_compound_literal, diag::ext_c99_feature, diag::ext_c99_flexible_array_member, diag::ext_c99_variable_decl_in_for_loop, diag::ext_c_nullptr, diag::ext_cast_fn_obj, diag::ext_clang_diagnose_if, diag::ext_clang_enable_if, diag::ext_cxx11_enum_fixed_underlying_type, diag::ext_designated_init, diag::ext_dtor_name_ambiguous, diag::ext_dtor_named_in_wrong_scope, diag::ext_duplicate_declspec, diag::ext_ellipsis_exception_spec, diag::ext_empty_fnmacro_arg, diag::ext_enum_value_not_int, diag::ext_enumerator_list_comma_c, diag::ext_enumerator_list_comma_cxx, diag::ext_explicit_instantiation_without_qualified_id, diag::ext_expr_not_ice, diag::ext_extern_template, diag::ext_extra_semi, diag::ext_forward_ref_enum, diag::ext_freestanding_complex, diag::ext_gnu_array_range, diag::ext_hex_constant_invalid, diag::ext_hex_literal_invalid, diag::ext_ident_list_in_param, diag::ext_in_class_initializer_non_constant, diag::ext_integer_complement_complex, diag::ext_integer_increment_complex, diag::ext_internal_in_extern_inline_quiet, diag::ext_line_comment, diag::ext_main_used, diag::ext_mixed_decls_code, diag::ext_multi_line_line_comment, diag::ext_named_variadic_macro, diag::ext_no_newline_eof, diag::ext_nonstandard_escape, diag::ext_ns_enum_attribute, diag::ext_opencl_double_without_pragma, diag::ext_pointer_to_const_ref_member_on_rvalue, diag::ext_pp_bad_vaargs_use, diag::ext_pp_comma_expr, diag::ext_pp_ident_directive, diag::ext_pp_line_too_big, diag::ext_pp_opencl_variadic_macros, diag::ext_pp_warning_directive, diag::ext_return_has_void_expr, diag::ext_rvalue_to_reference_access_ctor, diag::ext_rvalue_to_reference_temp_copy_no_viable, diag::ext_sizeof_alignof_function_type, diag::ext_sizeof_alignof_void_type, diag::ext_subscript_non_lvalue, diag::ext_subscript_overload, diag::ext_thread_before, diag::ext_typecheck_addrof_void, diag::ext_typecheck_cast_nonscalar, diag::ext_typecheck_comparison_of_fptr_to_void, diag::ext_typecheck_cond_one_void, diag::ext_typecheck_convert_pointer_void_func, diag::ext_typecheck_ordered_comparison_of_pointer_and_zero, diag::ext_variadic_macro, diag::warn_cxx20_compat_static_lambda, diag::warn_defined_in_function_type_macro, diag::warn_format_conversion_argument_type_mismatch_pedantic, diag::warn_kern_is_method, diag::warn_strict_prototypes, -1,
  /* DiagArray658 */ diag::warn_opencl_unsupported_core_feature, diag::warn_pragma_extension_is_core, -1,
  /* DiagArray660 */ diag::warn_pessimizing_move_on_initialization, diag::warn_pessimizing_move_on_return, -1,
  /* DiagArray661 */ diag::ext_sizeof_alignof_function_type, diag::ext_sizeof_alignof_void_type, diag::warn_sub_ptr_zero_size_types, -1,
  /* DiagArray662 */ diag::warn_cast_nonnull_to_bool, diag::warn_impcast_pointer_to_bool, -1,
  /* DiagArray663 */ diag::warn_pointer_compare, -1,
  /* DiagArray664 */ diag::ext_typecheck_comparison_of_pointer_integer, -1,
  /* DiagArray665 */ diag::ext_typecheck_convert_incompatible_pointer_sign, -1,
  /* DiagArray666 */ diag::warn_pointer_to_enum_cast, -1,
  /* DiagArray667 */ diag::warn_pointer_to_int_cast, -1,
  /* DiagArray668 */ diag::ext_typecheck_cond_incompatible_pointers, -1,
  /* DiagArray669 */ diag::warn_poison_system_directories, -1,
  /* DiagArray670 */ diag::warn_potentially_direct_selector_expression, -1,
  /* DiagArray671 */ diag::warn_side_effects_typeid, -1,
  /* DiagArray672 */ diag::warn_pragma_attribute_unused, -1,
  /* DiagArray673 */ diag::pp_pragma_once_in_main_file, -1,
  /* DiagArray674 */ diag::warn_pragma_pack_modified_after_include, diag::warn_pragma_pack_no_pop_eof, -1,
  /* DiagArray675 */ diag::warn_pragma_pack_non_default_at_include, -1,
  /* DiagArray676 */ diag::pp_pragma_sysheader_in_main_file, -1,
  /* DiagArray677 */ diag::warn_no_support_for_eval_method_source_on_m32, diag::warn_redefine_extname_not_applied, -1,
  /* DiagArray678 */ diag::warn_cxx11_compat_constexpr_body_invalid_stmt, diag::warn_cxx11_compat_constexpr_body_multiple_return, diag::warn_cxx11_compat_constexpr_body_no_return, diag::warn_cxx11_compat_constexpr_local_var, diag::warn_cxx11_compat_constexpr_type_definition, diag::warn_cxx11_compat_decltype_auto_type_specifier, diag::warn_cxx11_compat_deduced_return_type, diag::warn_cxx11_compat_digit_separator, diag::warn_cxx11_compat_generic_lambda, diag::warn_cxx11_compat_init_capture, diag::warn_cxx11_compat_variable_template, -1,
  /* DiagArray680 */ diag::warn_cxx14_compat_class_template_argument_deduction, diag::warn_cxx14_compat_constexpr_if, diag::warn_cxx14_compat_constexpr_on_lambda, diag::warn_cxx14_compat_decomp_decl, diag::warn_cxx14_compat_fold_expression, diag::warn_cxx14_compat_init_statement, diag::warn_cxx14_compat_inline_variable, diag::warn_cxx14_compat_nested_namespace_definition, diag::warn_cxx14_compat_star_this_lambda_capture, diag::warn_cxx14_compat_static_assert_no_message, diag::warn_cxx14_compat_template_nontype_parm_auto_type, diag::warn_cxx14_compat_template_template_param_typename, diag::warn_cxx14_compat_u8_character_literal, diag::warn_cxx14_compat_using_attribute_ns, diag::warn_cxx17_compat_multi_using_declaration, diag::warn_cxx17_compat_using_declaration_pack, diag::warn_for_range_begin_end_types_differ, -1,
  /* DiagArray681 */ diag::warn_cxx14_compat_ns_enum_attribute, diag::warn_cxx17_hex_literal, -1,
  /* DiagArray682 */ diag::warn_cxx17_compat_adl_only_template_id, diag::warn_cxx17_compat_bitfield_member_init, diag::warn_cxx17_compat_capture_binding, diag::warn_cxx17_compat_constexpr_body_invalid_stmt, diag::warn_cxx17_compat_constexpr_ctor_missing_init, diag::warn_cxx17_compat_constexpr_function_try_block, diag::warn_cxx17_compat_constexpr_local_var_no_init, diag::warn_cxx17_compat_constexpr_union_ctor_no_init, diag::warn_cxx17_compat_constexpr_virtual, diag::warn_cxx17_compat_decomp_decl_spec, diag::warn_cxx17_compat_defaulted_comparison, diag::warn_cxx17_compat_defaulted_method_type_mismatch, diag::warn_cxx17_compat_equals_this_lambda_capture, diag::warn_cxx17_compat_explicit_bool, diag::warn_cxx17_compat_for_range_init_stmt, diag::warn_cxx17_compat_init_capture_pack, diag::warn_cxx17_compat_inline_nested_namespace_definition, diag::warn_cxx17_compat_lambda_def_ctor_assign, diag::warn_cxx17_compat_lambda_template_parameter_list, diag::warn_cxx17_compat_missing_varargs_arg, diag::warn_cxx17_compat_spaceship, diag::warn_cxx17_compat_template_nontype_parm_type, diag::warn_cxx17_compat_unicode_type, diag::warn_cxx17_compat_using_decl_class_member_enumerator, diag::warn_cxx17_compat_using_decl_non_member_enumerator, diag::warn_cxx17_compat_using_decl_scoped_enumerator, diag::warn_cxx17_compat_using_enum_declaration, -1,
  /* DiagArray683 */ diag::warn_cxx17_compat_designated_init, diag::warn_cxx17_compat_pointer_to_const_ref_member_on_rvalue, -1,
  /* DiagArray684 */ diag::ext_subscript_overload, diag::warn_cxx20_alias_in_init_statement, diag::warn_cxx20_compat_auto_expr, diag::warn_cxx20_compat_consteval_if, diag::warn_cxx20_compat_constexpr_body_invalid_stmt, diag::warn_cxx20_compat_constexpr_var, diag::warn_cxx20_compat_decl_attrs_on_lambda, diag::warn_cxx20_compat_label_end_of_compound_statement, diag::warn_cxx20_compat_operator_overload_static, diag::warn_cxx20_compat_size_t_suffix, diag::warn_cxx20_compat_static_lambda, diag::warn_cxx2b_compat_pp_directive, diag::warn_cxx2b_compat_warning_directive, diag::warn_cxx2b_delimited_escape_sequence, -1,
  /* DiagArray686 */ diag::warn_c17_compat_bit_int, diag::warn_c17_compat_ellipsis_only_parameter, diag::warn_c17_compat_nullptr, diag::warn_c17_compat_static_assert_no_message, diag::warn_c2x_compat_bitint_suffix, diag::warn_c2x_compat_digit_separator, diag::warn_c2x_compat_label_end_of_compound_statement, diag::warn_c2x_compat_pp_directive, diag::warn_c2x_compat_typeof_type_specifier, diag::warn_c2x_compat_warning_directive, -1,
  /* DiagArray688 */ diag::warn_omp51_compat_attributes, -1,
  /* DiagArray689 */ diag::ext_predef_outside_function, -1,
  /* DiagArray690 */ diag::warn_private_extern, -1,
  /* DiagArray691 */ diag::warn_use_of_private_header_outside_module, -1,
  /* DiagArray692 */ diag::warn_mmap_mismatched_private_module_name, diag::warn_mmap_mismatched_private_submodule, diag::warn_mmap_redundant_export_as, diag::warn_no_priv_submodule_use_toplevel, -1,
  /* DiagArray693 */ diag::warn_profile_data_missing, -1,
  /* DiagArray694 */ diag::warn_profile_data_out_of_date, -1,
  /* DiagArray695 */ diag::warn_profile_data_unprofiled, -1,
  /* DiagArray696 */ diag::warn_property_access_suggest, -1,
  /* DiagArray697 */ diag::warn_property_attr_mismatch, diag::warn_property_attribute, diag::warn_property_redecl_getter_mismatch, diag::warn_readonly_property, -1,
  /* DiagArray698 */ diag::warn_unimplemented_protocol_method, -1,
  /* DiagArray699 */ diag::warn_protocol_property_mismatch, -1,
  /* DiagArray700 */ diag::warn_avx_calling_convention, -1,
  /* DiagArray701 */ diag::err_func_returning_qualified_void, -1,
  /* DiagArray702 */ diag::warn_quoted_include_in_framework_header, -1,
  /* DiagArray704 */ diag::warn_for_range_ref_binds_ret_temp, -1,
  /* DiagArray705 */ diag::warn_for_range_const_ref_binds_temp_built_from_ref, diag::warn_for_range_copy, -1,
  /* DiagArray706 */ diag::warn_var_decl_not_read_only, -1,
  /* DiagArray707 */ diag::warn_auto_readonly_iboutlet_property, -1,
  /* DiagArray708 */ diag::warn_bad_receiver_type, -1,
  /* DiagArray709 */ diag::warn_receiver_forward_class, diag::warn_receiver_forward_instance, -1,
  /* DiagArray710 */ diag::ext_member_redeclared, -1,
  /* DiagArray711 */ diag::warn_consteval_if_always_true, -1,
  /* DiagArray713 */ diag::warn_redundant_move_on_return, -1,
  /* DiagArray714 */ diag::warn_redundant_parens_around_declarator, -1,
  /* DiagArray715 */ diag::ext_register_storage_class, -1,
  /* DiagArray716 */ diag::warn_reinterpret_different_from_static, -1,
  /* DiagArray717 */ diag::remark_fe_backend_plugin, -1,
  /* DiagArray719 */ diag::warn_initializer_out_of_order, diag::warn_some_initializers_out_of_order, -1,
  /* DiagArray720 */ diag::ext_designated_init_reordered, -1,
  /* DiagArray721 */ diag::warn_objc_requires_super_protocol, -1,
  /* DiagArray723 */ diag::warn_reserved_extern_symbol, -1,
  /* DiagArray724 */ diag::warn_pp_macro_is_reserved_id, -1,
  /* DiagArray725 */ diag::ext_ms_reserved_user_defined_literal, diag::ext_reserved_user_defined_literal, -1,
  /* DiagArray726 */ diag::warn_pragma_restrict_expansion_macro_use, -1,
  /* DiagArray727 */ diag::ext_retained_language_linkage, -1,
  /* DiagArray728 */ diag::warn_ret_addr_label, diag::warn_ret_local_temp_addr_ref, diag::warn_ret_stack_addr_ref, -1,
  /* DiagArray730 */ diag::ext_return_has_expr, diag::ext_return_missing_expr, diag::warn_falloff_nonvoid_coroutine, diag::warn_falloff_nonvoid_function, diag::warn_falloff_nonvoid_lambda, diag::warn_maybe_falloff_nonvoid_coroutine, diag::warn_maybe_falloff_nonvoid_function, diag::warn_maybe_falloff_nonvoid_lambda, diag::warn_return_missing_expr, -1,
  /* DiagArray731 */ diag::warn_return_value_udt, diag::warn_return_value_udt_incomplete, -1,
  /* DiagArray732 */ diag::ext_ovl_rewrite_equalequal_not_bool, -1,
  /* DiagArray733 */ diag::remark_cc1_round_trip_generated, -1,
  /* DiagArray734 */ diag::warn_no_dynamic_cast_with_rtti_disabled, diag::warn_no_typeid_with_rtti_disabled, -1,
  /* DiagArray735 */ diag::remark_sanitize_address_insert_extra_padding_accepted, diag::remark_sanitize_address_insert_extra_padding_rejected, -1,
  /* DiagArray736 */ diag::warn_drv_sarif_format_unstable, -1,
  /* DiagArray737 */ diag::remark_pp_search_path_usage, -1,
  /* DiagArray738 */ diag::warn_attribute_section_on_redeclaration, diag::warn_duplicate_codeseg_attribute, diag::warn_mismatched_section, -1,
  /* DiagArray739 */ diag::warn_unimplemented_selector, -1,
  /* DiagArray740 */ diag::warn_multiple_selectors, -1,
  /* DiagArray741 */ diag::warn_self_assignment_builtin, -1,
  /* DiagArray742 */ diag::warn_identity_field_assign, -1,
  /* DiagArray743 */ diag::warn_self_assignment_overloaded, -1,
  /* DiagArray744 */ diag::warn_self_move, -1,
  /* DiagArray745 */ diag::warn_semicolon_before_method_body, -1,
  /* DiagArray746 */ diag::warn_missing_sentinel, diag::warn_not_enough_argument, -1,
  /* DiagArray748 */ diag::warn_fe_serialized_diag_failure, diag::warn_fe_serialized_diag_failure_during_finalisation, diag::warn_fe_serialized_diag_merge_failure, -1,
  /* DiagArray749 */ diag::warn_decl_shadow, -1,
  /* DiagArray751 */ diag::warn_shadow_field, -1,
  /* DiagArray752 */ diag::warn_ctor_parm_shadows_field, -1,
  /* DiagArray753 */ diag::warn_modifying_shadowing_decl, -1,
  /* DiagArray754 */ diag::warn_ivar_use_hidden, -1,
  /* DiagArray755 */ diag::warn_decl_shadow_uncaptured_local, -1,
  /* DiagArray756 */ diag::warn_shift_negative, -1,
  /* DiagArray757 */ diag::warn_shift_gt_typewidth, -1,
  /* DiagArray758 */ diag::warn_shift_lhs_negative, -1,
  /* DiagArray759 */ diag::warn_addition_in_bitshift, -1,
  /* DiagArray760 */ diag::warn_shift_result_gt_typewidth, -1,
  /* DiagArray761 */ diag::warn_shift_result_sets_sign_bit, -1,
  /* DiagArray762 */ diag::warn_impcast_integer_64_32, -1,
  /* DiagArray763 */ diag::warn_mixed_sign_comparison, -1,
  /* DiagArray764 */ diag::warn_impcast_integer_sign, diag::warn_impcast_integer_sign_conditional, diag::warn_impcast_nonnegative_result, -1,
  /* DiagArray766 */ diag::warn_no_underlying_type_specified_for_enum_bitfield, -1,
  /* DiagArray767 */ diag::ext_wchar_t_sign_spec, -1,
  /* DiagArray768 */ diag::warn_impcast_single_bit_bitield_precision_constant, -1,
  /* DiagArray769 */ diag::warn_sizeof_array_param, -1,
  /* DiagArray770 */ diag::warn_sizeof_array_decay, -1,
  /* DiagArray771 */ diag::warn_division_sizeof_array, -1,
  /* DiagArray772 */ diag::warn_division_sizeof_ptr, -1,
  /* DiagArray773 */ diag::warn_sizeof_pointer_expr_memaccess, diag::warn_sizeof_pointer_type_memaccess, -1,
  /* DiagArray774 */ diag::warn_slash_u_filename, -1,
  /* DiagArray775 */ diag::warn_slh_does_not_support_asm_goto, -1,
  /* DiagArray776 */ diag::remark_sloc_usage, -1,
  /* DiagArray777 */ diag::warn_sometimes_uninit_var, -1,
  /* DiagArray778 */ diag::warn_fe_source_mgr, -1,
  /* DiagArray779 */ diag::warn_omp_declare_variant_after_emitted, diag::warn_omp_declare_variant_after_used, diag::warn_omp_declare_variant_marked_as_declare_variant, diag::warn_omp_declare_variant_score_not_constant, diag::warn_omp_nesting_simd, diag::warn_omp_unterminated_declare_target, diag::warn_pragma_omp_ignored, diag::warn_unknown_declare_variant_isa_trait, -1,
  /* DiagArray780 */ diag::warn_sampler_initializer_invalid_bits, -1,
  /* DiagArray782 */ diag::warn_stack_exhausted, -1,
  /* DiagArray783 */ diag::warn_stack_clash_protection_inline_asm, -1,
  /* DiagArray784 */ diag::ext_in_class_initializer_float_type_cxx11, -1,
  /* DiagArray785 */ diag::ext_internal_in_extern_inline, diag::ext_internal_in_extern_inline_quiet, -1,
  /* DiagArray786 */ diag::warn_static_inline_explicit_inst_ignored, -1,
  /* DiagArray787 */ diag::warn_static_local_in_extern_inline, -1,
  /* DiagArray788 */ diag::warn_static_self_reference_in_init, -1,
  /* DiagArray789 */ diag::warn_drv_libstdcxx_not_found, -1,
  /* DiagArray801 */ diag::warn_strict_potentially_direct_selector_expression, -1,
  /* DiagArray802 */ diag::warn_strict_prototypes, -1,
  /* DiagArray803 */ diag::warn_strict_multiple_method_decl, -1,
  /* DiagArray804 */ diag::warn_stringcompare, -1,
  /* DiagArray805 */ diag::warn_concatenated_literal_array_init, -1,
  /* DiagArray806 */ diag::warn_impcast_string_literal_to_bool, -1,
  /* DiagArray807 */ diag::warn_string_plus_char, -1,
  /* DiagArray808 */ diag::warn_string_plus_int, -1,
  /* DiagArray809 */ diag::warn_strlcpycat_wrong_size, -1,
  /* DiagArray810 */ diag::warn_strncat_large_size, diag::warn_strncat_src_size, diag::warn_strncat_wrong_size, -1,
  /* DiagArray811 */ diag::warn_suggest_destructor_marked_not_override_overriding, -1,
  /* DiagArray812 */ diag::warn_suggest_function_marked_not_override_overriding, -1,
  /* DiagArray813 */ diag::ext_typecheck_base_super, -1,
  /* DiagArray814 */ diag::warn_suspicious_bzero_size, -1,
  /* DiagArray816 */ diag::warn_attr_swift_name_decl_kind, diag::warn_attr_swift_name_decl_missing_params, diag::warn_attr_swift_name_function, diag::warn_attr_swift_name_getter_parameters, diag::warn_attr_swift_name_invalid_identifier, diag::warn_attr_swift_name_missing_parameters, diag::warn_attr_swift_name_multiple_selfs, diag::warn_attr_swift_name_num_params, diag::warn_attr_swift_name_setter_parameters, diag::warn_attr_swift_name_subscript_getter_newValue, diag::warn_attr_swift_name_subscript_invalid_parameter, diag::warn_attr_swift_name_subscript_setter_multiple_newValues, diag::warn_attr_swift_name_subscript_setter_no_newValue, -1,
  /* DiagArray817 */ diag::warn_case_value_overflow, diag::warn_missing_case, diag::warn_not_in_enum, -1,
  /* DiagArray818 */ diag::warn_bool_switch_condition, -1,
  /* DiagArray820 */ diag::warn_def_missing_case, -1,
  /* DiagArray821 */ diag::warn_sync_fetch_and_nand_semantics_change, -1,
  /* DiagArray823 */ diag::warn_target_clone_mixed_values, -1,
  /* DiagArray824 */ diag::warn_comparison_bitwise_always, diag::warn_comparison_bitwise_or, -1,
  /* DiagArray825 */ diag::warn_alignment_builtin_useless, diag::warn_comparison_always, -1,
  /* DiagArray826 */ diag::warn_integer_constants_in_conditional_always_true, diag::warn_left_shift_always, diag::warn_tautological_bool_compare, -1,
  /* DiagArray828 */ diag::warn_out_of_range_compare, -1,
  /* DiagArray829 */ diag::warn_tautological_compare_objc_bool, -1,
  /* DiagArray830 */ diag::warn_tautological_overlap_comparison, -1,
  /* DiagArray831 */ diag::warn_nonnull_expr_compare, diag::warn_null_pointer_compare, -1,
  /* DiagArray832 */ diag::warn_tautological_constant_compare, -1,
  /* DiagArray833 */ diag::warn_address_of_reference_null_compare, diag::warn_this_null_compare, -1,
  /* DiagArray834 */ diag::warn_unsigned_char_always_true_comparison, -1,
  /* DiagArray835 */ diag::warn_unsigned_enum_always_true_comparison, -1,
  /* DiagArray836 */ diag::warn_unsigned_always_true_comparison, -1,
  /* DiagArray837 */ diag::warn_tautological_compare_value_range, -1,
  /* DiagArray838 */ diag::warn_tcb_enforcement_violation, -1,
  /* DiagArray839 */ diag::ext_typecheck_decl_incomplete_type, -1,
  /* DiagArray841 */ diag::warn_acquired_before, diag::warn_acquired_before_after_cycle, diag::warn_cannot_resolve_lock, diag::warn_double_lock, diag::warn_expecting_lock_held_on_loop, diag::warn_expecting_locked, diag::warn_fun_excludes_mutex, diag::warn_fun_requires_lock, diag::warn_fun_requires_negative_cap, diag::warn_lock_exclusive_and_shared, diag::warn_lock_some_predecessors, diag::warn_no_unlock, diag::warn_unlock_but_no_lock, diag::warn_unlock_kind_mismatch, diag::warn_var_deref_requires_any_lock, diag::warn_var_deref_requires_lock, diag::warn_variable_requires_any_lock, diag::warn_variable_requires_lock, -1,
  /* DiagArray842 */ diag::warn_thread_attribute_argument_not_lockable, diag::warn_thread_attribute_decl_not_lockable, diag::warn_thread_attribute_decl_not_pointer, diag::warn_thread_attribute_ignored, diag::warn_thread_attribute_not_on_capability_member, diag::warn_thread_attribute_not_on_non_static_member, -1,
  /* DiagArray843 */ diag::warn_thread_safety_beta, -1,
  /* DiagArray844 */ diag::warn_acquire_requires_negative_cap, -1,
  /* DiagArray845 */ diag::warn_fun_requires_lock_precise, diag::warn_var_deref_requires_lock_precise, diag::warn_variable_requires_lock_precise, -1,
  /* DiagArray846 */ diag::warn_guarded_pass_by_reference, diag::warn_pt_guarded_pass_by_reference, -1,
  /* DiagArray847 */ diag::warn_thread_safety_verbose, -1,
  /* DiagArray848 */ diag::trigraph_converted, diag::trigraph_ends_block_comment, diag::trigraph_ignored, diag::trigraph_ignored_block_comment, -1,
  /* DiagArray850 */ diag::warn_type_safety_null_pointer_required, diag::warn_type_safety_type_mismatch, diag::warn_type_tag_for_datatype_wrong_kind, -1,
  /* DiagArray851 */ diag::ext_redefinition_of_typedef, -1,
  /* DiagArray852 */ diag::ext_typename_missing, -1,
  /* DiagArray853 */ diag::warn_fe_unable_to_open_stats_file, -1,
  /* DiagArray854 */ diag::warn_unaligned_access, -1,
  /* DiagArray855 */ diag::warn_imp_cast_drops_unaligned, -1,
  /* DiagArray856 */ diag::warn_unavailable_fwdclass_message, -1,
  /* DiagArray857 */ diag::warn_undeclared_selector, diag::warn_undeclared_selector_with_typo, -1,
  /* DiagArray858 */ diag::warn_pp_undef_identifier, -1,
  /* DiagArray859 */ diag::warn_pp_undef_prefix, -1,
  /* DiagArray860 */ diag::warn_address_of_reference_bool_conversion, diag::warn_this_bool_conversion, -1,
  /* DiagArray861 */ diag::warn_func_template_missing, -1,
  /* DiagArray862 */ diag::warn_undefined_inline, -1,
  /* DiagArray863 */ diag::warn_undefined_internal, -1,
  /* DiagArray864 */ diag::ext_undefined_internal_type, -1,
  /* DiagArray865 */ diag::warn_pointer_indirection_from_incompatible_type, diag::warn_undefined_reinterpret_cast, -1,
  /* DiagArray866 */ diag::warn_var_template_missing, -1,
  /* DiagArray867 */ diag::warn_throw_underaligned_obj, -1,
  /* DiagArray868 */ diag::warn_side_effects_unevaluated_context, -1,
  /* DiagArray869 */ diag::warn_unguarded_availability, -1,
  /* DiagArray870 */ diag::warn_unguarded_availability_new, -1,
  /* DiagArray871 */ diag::warn_delimited_ucn_empty, diag::warn_delimited_ucn_incomplete, diag::warn_ucn_escape_incomplete, diag::warn_ucn_escape_no_digits, diag::warn_ucn_escape_surrogate, diag::warn_ucn_not_valid_in_c89, diag::warn_ucn_not_valid_in_c89_literal, -1,
  /* DiagArray872 */ diag::warn_utf8_symbol_homoglyph, -1,
  /* DiagArray873 */ diag::ext_unicode_whitespace, -1,
  /* DiagArray874 */ diag::warn_utf8_symbol_zero_width, -1,
  /* DiagArray875 */ diag::warn_base_class_is_uninit, diag::warn_field_is_uninit, diag::warn_reference_field_is_uninit, diag::warn_uninit_byref_blockvar_captured_by_block, diag::warn_uninit_self_reference_in_init, diag::warn_uninit_self_reference_in_reference_init, diag::warn_uninit_var, -1,
  /* DiagArray876 */ diag::warn_uninit_const_reference, -1,
  /* DiagArray877 */ diag::warn_drv_potentially_misspelled_joined_argument, diag::warn_drv_unknown_argument_clang_cl, diag::warn_drv_unknown_argument_clang_cl_with_suggestion, -1,
  /* DiagArray878 */ diag::warn_assume_attribute_string_unknown, -1,
  /* DiagArray879 */ diag::warn_unknown_attribute_ignored, -1,
  /* DiagArray880 */ diag::warn_drv_new_cuda_version, diag::warn_drv_partially_supported_cuda_version, -1,
  /* DiagArray881 */ diag::warn_pp_invalid_directive, -1,
  /* DiagArray882 */ diag::ext_unknown_escape, -1,
  /* DiagArray883 */ diag::ext_on_off_switch_syntax, diag::ext_pragma_syntax_eod, diag::ext_stdc_pragma_ignored, diag::warn_pragma_diagnostic_cannot_pop, diag::warn_pragma_diagnostic_invalid, diag::warn_pragma_diagnostic_invalid_option, diag::warn_pragma_diagnostic_invalid_token, diag::warn_pragma_exec_charset_expected, diag::warn_pragma_exec_charset_push_invalid, diag::warn_pragma_exec_charset_spec_invalid, diag::warn_pragma_ignored, diag::warn_pragma_include_alias_expected, diag::warn_pragma_include_alias_expected_filename, diag::warn_pragma_include_alias_mismatch_angle, diag::warn_pragma_include_alias_mismatch_quote, diag::warn_pragma_warning_expected, diag::warn_pragma_warning_expected_number, diag::warn_pragma_warning_push_level, diag::warn_pragma_warning_spec_invalid, diag::warn_stdc_fenv_round_not_supported, -1,
  /* DiagArray884 */ diag::warn_unknown_sanitizer_ignored, -1,
  /* DiagArray885 */ diag::warn_pragma_diagnostic_unknown_warning, diag::warn_unknown_diag_option, diag::warn_unknown_warning_specifier, -1,
  /* DiagArray886 */ diag::ext_template_arg_unnamed_type, -1,
  /* DiagArray887 */ diag::warn_unneeded_internal_decl, diag::warn_unneeded_static_internal_decl, -1,
  /* DiagArray888 */ diag::warn_unneeded_member_function, -1,
  /* DiagArray889 */ diag::warn_unqualified_call_to_std_cast_function, -1,
  /* DiagArray890 */ diag::warn_unreachable, -1,
  /* DiagArray892 */ diag::warn_unreachable_break, -1,
  /* DiagArray893 */ diag::warn_unreachable_fallthrough_attr, -1,
  /* DiagArray894 */ diag::warn_unreachable_association, -1,
  /* DiagArray895 */ diag::warn_unreachable_loop_increment, -1,
  /* DiagArray896 */ diag::warn_unreachable_return, -1,
  /* DiagArray897 */ diag::warn_unsafe_buffer_expression, diag::warn_unsafe_buffer_variable, -1,
  /* DiagArray898 */ diag::warn_unsequenced_mod_mod, diag::warn_unsequenced_mod_use, -1,
  /* DiagArray899 */ diag::warn_drv_unsupported_float_abi_by_lib, -1,
  /* DiagArray900 */ diag::warn_target_unsupported_abs2008, diag::warn_target_unsupported_abslegacy, -1,
  /* DiagArray901 */ diag::warn_at_available_unchecked_use, -1,
  /* DiagArray902 */ diag::warn_target_unsupported_compact_branches, -1,
  /* DiagArray903 */ diag::warn_attribute_dll_instantiated_base_class, -1,
  /* DiagArray904 */ diag::warn_fe_backend_unsupported_fp_exceptions, diag::warn_fe_backend_unsupported_fp_rounding, -1,
  /* DiagArray905 */ diag::warn_template_qualified_friend_ignored, diag::warn_template_qualified_friend_unsupported, -1,
  /* DiagArray906 */ diag::warn_drv_unsupported_gpopt, -1,
  /* DiagArray907 */ diag::warn_target_unsupported_nan2008, diag::warn_target_unsupported_nanlegacy, -1,
  /* DiagArray908 */ diag::warn_drv_dwarf_version_limited_by_target, diag::warn_drv_unsupported_debug_info_opt_for_target, -1,
  /* DiagArray909 */ diag::warn_attribute_protected_visibility, -1,
  /* DiagArray910 */ diag::ext_partial_specs_not_deducible, -1,
  /* DiagArray913 */ diag::warn_unused_but_set_parameter, -1,
  /* DiagArray914 */ diag::warn_unused_but_set_variable, -1,
  /* DiagArray915 */ diag::warn_drv_diagnostics_hotness_requires_pgo, diag::warn_drv_diagnostics_misexpect_requires_pgo, diag::warn_drv_empty_joined_argument, diag::warn_drv_input_file_unused, diag::warn_drv_input_file_unused_by_cpp, diag::warn_drv_invalid_arch_name_with_suggestion, diag::warn_drv_preprocessed_input_file_unused, diag::warn_drv_unused_argument, diag::warn_drv_unused_x, diag::warn_ignored_clang_option, diag::warn_ignoring_fdiscard_for_bitcode, diag::warn_ignoring_verify_debuginfo_preserve_export, -1,
  /* DiagArray916 */ diag::warn_unused_comparison, -1,
  /* DiagArray917 */ diag::warn_unused_const_variable, -1,
  /* DiagArray918 */ diag::warn_unused_exception_param, -1,
  /* DiagArray919 */ diag::warn_unused_function, -1,
  /* DiagArray920 */ diag::warn_unused_property_expr, -1,
  /* DiagArray921 */ diag::warn_unused_label, -1,
  /* DiagArray922 */ diag::warn_unused_lambda_capture, -1,
  /* DiagArray923 */ diag::warn_unused_local_typedef, -1,
  /* DiagArray925 */ diag::pp_macro_not_used, -1,
  /* DiagArray926 */ diag::warn_unused_member_function, -1,
  /* DiagArray927 */ diag::warn_unused_parameter, -1,
  /* DiagArray928 */ diag::warn_unused_private_field, -1,
  /* DiagArray929 */ diag::warn_unused_property_backing_ivar, -1,
  /* DiagArray930 */ diag::warn_unused_result, diag::warn_unused_result_msg, -1,
  /* DiagArray931 */ diag::warn_unused_template, -1,
  /* DiagArray932 */ diag::warn_unused_call, diag::warn_unused_comma_left_operand, diag::warn_unused_constructor, diag::warn_unused_constructor_msg, diag::warn_unused_container_subscript_expr, diag::warn_unused_expr, diag::warn_unused_voidptr, -1,
  /* DiagArray933 */ diag::warn_unused_variable, -1,
  /* DiagArray934 */ diag::warn_unused_volatile, -1,
  /* DiagArray935 */ diag::warn_used_but_marked_unused, -1,
  /* DiagArray936 */ diag::warn_user_literal_reserved, -1,
  /* DiagArray937 */ diag::warn_diagnose_if_succeeded, -1,
  /* DiagArray938 */ diag::warn_second_arg_of_va_start_not_last_named_param, diag::warn_second_parameter_to_va_arg_never_compatible, diag::warn_va_start_type_is_undefined, -1,
  /* DiagArray939 */ diag::ext_named_variadic_macro, diag::ext_pp_bad_vaopt_use, diag::ext_variadic_macro, -1,
  /* DiagArray940 */ diag::warn_typecheck_vector_element_sizes_not_equal, -1,
  /* DiagArray941 */ diag::warn_incompatible_vectors, -1,
  /* DiagArray943 */ diag::warn_empty_parens_are_function_decl, diag::warn_parens_disambiguated_as_function_declaration, diag::warn_parens_disambiguated_as_variable_declaration, -1,
  /* DiagArray944 */ diag::warn_decl_in_param_list, diag::warn_redefinition_in_param_list, -1,
  /* DiagArray945 */ diag::warn_vla_used, -1,
  /* DiagArray946 */ diag::ext_vla, -1,
  /* DiagArray947 */ diag::warn_void_pointer_to_enum_cast, -1,
  /* DiagArray948 */ diag::warn_void_pointer_to_int_cast, -1,
  /* DiagArray949 */ diag::ext_typecheck_indirection_through_void_pointer, diag::ext_typecheck_indirection_through_void_pointer_cpp, -1,
  /* DiagArray951 */ diag::warn_wasm_dynamic_exception_spec_ignored, -1,
  /* DiagArray952 */ diag::warn_weak_template_vtable, -1,
  /* DiagArray953 */ diag::warn_weak_vtable, -1,
  /* DiagArray954 */ diag::ext_deprecated_string_literal_conversion, -1,
  /* DiagArray956 */ diag::warn_xor_used_as_pow, diag::warn_xor_used_as_pow_base, diag::warn_xor_used_as_pow_base_extra, -1,
  /* DiagArray957 */ diag::warn_zero_as_null_pointer_constant, -1,
  /* DiagArray958 */ diag::ext_typecheck_zero_array_size, -1,
};

static const int16_t DiagSubGroups[] = {
  /* Empty */ -1,
  /* DiagSubGroup0 */ 293, -1,
  /* DiagSubGroup4 */ 17, 293, -1,
  /* DiagSubGroup11 */ 662, 804, 831, -1,
  /* DiagSubGroup17 */ 542, 649, 817, 818, 511, -1,
  /* DiagSubGroup27 */ 208, -1,
  /* DiagSubGroup29 */ 36, 35, 32, -1,
  /* DiagSubGroup34 */ 31, -1,
  /* DiagSubGroup42 */ 43, -1,
  /* DiagSubGroup52 */ 383, 186, -1,
  /* DiagSubGroup56 */ 879, 374, -1,
  /* DiagSubGroup66 */ 101, 128, 341, -1,
  /* DiagSubGroup67 */ 131, -1,
  /* DiagSubGroup70 */ 768, -1,
  /* DiagSubGroup77 */ 662, 860, -1,
  /* DiagSubGroup78 */ 77, -1,
  /* DiagSubGroup79 */ 74, -1,
  /* DiagSubGroup88 */ 91, -1,
  /* DiagSubGroup89 */ 95, -1,
  /* DiagSubGroup90 */ 99, -1,
  /* DiagSubGroup91 */ 99, 94, 92, 678, 680, 682, 684, -1,
  /* DiagSubGroup93 */ 91, 679, 681, 683, 685, -1,
  /* DiagSubGroup95 */ 96, 97, 98, -1,
  /* DiagSubGroup102 */ 680, 682, 684, -1,
  /* DiagSubGroup103 */ 102, 681, 683, 685, -1,
  /* DiagSubGroup104 */ 101, 100, -1,
  /* DiagSubGroup106 */ 233, 227, 107, 682, 684, -1,
  /* DiagSubGroup108 */ 106, 683, 685, -1,
  /* DiagSubGroup109 */ 105, -1,
  /* DiagSubGroup110 */ 104, -1,
  /* DiagSubGroup111 */ 106, -1,
  /* DiagSubGroup112 */ 107, -1,
  /* DiagSubGroup113 */ 109, -1,
  /* DiagSubGroup115 */ 684, -1,
  /* DiagSubGroup116 */ 115, 685, -1,
  /* DiagSubGroup118 */ 117, 114, -1,
  /* DiagSubGroup119 */ 115, -1,
  /* DiagSubGroup120 */ 116, -1,
  /* DiagSubGroup121 */ 118, -1,
  /* DiagSubGroup123 */ 682, -1,
  /* DiagSubGroup124 */ 683, -1,
  /* DiagSubGroup125 */ 680, -1,
  /* DiagSubGroup126 */ 681, -1,
  /* DiagSubGroup127 */ 678, -1,
  /* DiagSubGroup129 */ 679, -1,
  /* DiagSubGroup130 */ 133, 135, 678, 680, 682, 684, -1,
  /* DiagSubGroup134 */ 130, 131, 132, 679, 681, 683, 685, -1,
  /* DiagSubGroup140 */ 117, -1,
  /* DiagSubGroup141 */ 140, -1,
  /* DiagSubGroup143 */ 161, -1,
  /* DiagSubGroup146 */ 147, -1,
  /* DiagSubGroup155 */ 560, -1,
  /* DiagSubGroup159 */ 158, -1,
  /* DiagSubGroup163 */ 164, 165, -1,
  /* DiagSubGroup169 */ 70, 585, -1,
  /* DiagSubGroup174 */ 77, 169, 279, 71, 304, 762, 429, 393, 390, 459, 557, 573, 595, 764, 806, -1,
  /* DiagSubGroup175 */ 573, -1,
  /* DiagSubGroup177 */ 178, 218, 20, 176, -1,
  /* DiagSubGroup180 */ 2, -1,
  /* DiagSubGroup188 */ 190, 192, 191, 728, -1,
  /* DiagSubGroup203 */ 202, 200, -1,
  /* DiagSubGroup206 */ 208, 209, 210, 212, 213, 215, 219, 220, 221, 222, 223, 224, 211, 227, 232, 233, 235, 236, 237, 238, -1,
  /* DiagSubGroup213 */ 216, -1,
  /* DiagSubGroup214 */ 215, -1,
  /* DiagSubGroup215 */ 217, -1,
  /* DiagSubGroup218 */ 225, -1,
  /* DiagSubGroup230 */ 231, -1,
  /* DiagSubGroup238 */ 92, -1,
  /* DiagSubGroup244 */ 245, -1,
  /* DiagSubGroup249 */ 251, 250, -1,
  /* DiagSubGroup252 */ 253, -1,
  /* DiagSubGroup264 */ 220, -1,
  /* DiagSubGroup265 */ 562, -1,
  /* DiagSubGroup274 */ 297, -1,
  /* DiagSubGroup275 */ 277, 221, -1,
  /* DiagSubGroup276 */ 222, -1,
  /* DiagSubGroup279 */ 280, 281, 276, -1,
  /* DiagSubGroup280 */ 223, -1,
  /* DiagSubGroup281 */ 224, -1,
  /* DiagSubGroup293 */ 213, 520, 380, 422, 745, 523, 763, 927, 913, 575, 576, 271, 805, 330, -1,
  /* DiagSubGroup295 */ 132, 96, -1,
  /* DiagSubGroup296 */ 271, -1,
  /* DiagSubGroup304 */ 306, 307, -1,
  /* DiagSubGroup309 */ 310, 319, 563, 316, 318, 312, 311, -1,
  /* DiagSubGroup320 */ 314, 316, 318, -1,
  /* DiagSubGroup325 */ 324, -1,
  /* DiagSubGroup329 */ 823, -1,
  /* DiagSubGroup331 */ 100, 105, 114, -1,
  /* DiagSubGroup336 */ 337, 338, 340, 341, 342, 343, 344, 345, 346, 347, 348, 946, 349, 350, 351, 352, 353, 355, 356, 357, 358, 710, 359, 360, 362, 363, 364, 365, 958, 366, 367, -1,
  /* DiagSubGroup360 */ 361, -1,
  /* DiagSubGroup379 */ 377, 378, -1,
  /* DiagSubGroup380 */ 381, -1,
  /* DiagSubGroup382 */ 391, 392, -1,
  /* DiagSubGroup387 */ 388, -1,
  /* DiagSubGroup390 */ 394, 618, -1,
  /* DiagSubGroup393 */ 619, -1,
  /* DiagSubGroup394 */ 384, -1,
  /* DiagSubGroup407 */ 408, 403, -1,
  /* DiagSubGroup413 */ 415, 559, -1,
  /* DiagSubGroup419 */ 227, -1,
  /* DiagSubGroup430 */ 429, -1,
  /* DiagSubGroup432 */ 433, -1,
  /* DiagSubGroup436 */ 376, -1,
  /* DiagSubGroup461 */ 133, -1,
  /* DiagSubGroup464 */ 98, -1,
  /* DiagSubGroup465 */ 308, 703, -1,
  /* DiagSubGroup477 */ 481, 486, 498, 484, 494, 502, 478, 507, 490, 508, 499, 500, 506, 492, 489, 485, 504, 501, 488, 496, 495, 493, 480, 483, 509, 479, 482, 487, 503, 416, -1,
  /* DiagSubGroup504 */ 505, -1,
  /* DiagSubGroup542 */ 41, 79, 152, 158, 203, 309, 308, 323, 382, 420, 431, 515, 516, 543, 546, 705, 718, 730, 741, 744, 769, 770, 808, 825, 848, 875, 883, 911, 950, 600, 589, 591, 641, 690, 148, 291, 937, -1,
  /* DiagSubGroup543 */ 660, 713, 729, 744, -1,
  /* DiagSubGroup544 */ 498, -1,
  /* DiagSubGroup548 */ 99, -1,
  /* DiagSubGroup554 */ 107, -1,
  /* DiagSubGroup556 */ 763, 174, 460, -1,
  /* DiagSubGroup559 */ 558, -1,
  /* DiagSubGroup575 */ 357, -1,
  /* DiagSubGroup578 */ 579, -1,
  /* DiagSubGroup588 */ 614, -1,
  /* DiagSubGroup594 */ 620, -1,
  /* DiagSubGroup614 */ 615, -1,
  /* DiagSubGroup617 */ 619, 618, 585, 829, -1,
  /* DiagSubGroup627 */ 779, 629, 630, 632, 631, 628, -1,
  /* DiagSubGroup632 */ 631, -1,
  /* DiagSubGroup642 */ 422, -1,
  /* DiagSubGroup646 */ 647, -1,
  /* DiagSubGroup649 */ 463, 462, 73, 75, 759, 640, 650, 189, -1,
  /* DiagSubGroup651 */ 869, -1,
  /* DiagSubGroup657 */ 136, 96, 98, 100, 101, 105, 114, 117, 303, 331, 338, 340, 341, 342, 343, 344, 345, 347, 348, 349, 350, 352, 353, 355, 356, 357, 358, 359, 360, 364, 366, 367, 453, 464, 481, 482, 484, 487, 489, 494, 495, 501, 639, 946, 958, 204, 398, 268, 272, 581, 300, 69, 864, 727, 162, 549, 254, 455, 450, -1,
  /* DiagSubGroup659 */ 232, 466, 84, 726, 299, -1,
  /* DiagSubGroup661 */ 358, -1,
  /* DiagSubGroup666 */ 947, -1,
  /* DiagSubGroup667 */ 666, 948, -1,
  /* DiagSubGroup674 */ 675, -1,
  /* DiagSubGroup677 */ 883, 379, 672, 674, -1,
  /* DiagSubGroup679 */ 678, 128, -1,
  /* DiagSubGroup681 */ 680, -1,
  /* DiagSubGroup683 */ 682, -1,
  /* DiagSubGroup685 */ 684, -1,
  /* DiagSubGroup687 */ 686, -1,
  /* DiagSubGroup703 */ 705, 704, -1,
  /* DiagSubGroup715 */ 233, -1,
  /* DiagSubGroup718 */ 719, 720, -1,
  /* DiagSubGroup722 */ 724, -1,
  /* DiagSubGroup723 */ 724, -1,
  /* DiagSubGroup725 */ 94, -1,
  /* DiagSubGroup730 */ 731, -1,
  /* DiagSubGroup739 */ 740, -1,
  /* DiagSubGroup741 */ 743, 742, -1,
  /* DiagSubGroup747 */ 898, -1,
  /* DiagSubGroup749 */ 753, 754, -1,
  /* DiagSubGroup750 */ 749, 752, 755, 751, -1,
  /* DiagSubGroup752 */ 753, -1,
  /* DiagSubGroup781 */ 780, -1,
  /* DiagSubGroup784 */ 362, -1,
  /* DiagSubGroup801 */ 670, -1,
  /* DiagSubGroup802 */ 228, -1,
  /* DiagSubGroup815 */ 773, 263, 568, 474, 814, -1,
  /* DiagSubGroup825 */ 826, 831, 830, 824, 833, 829, -1,
  /* DiagSubGroup826 */ 828, -1,
  /* DiagSubGroup827 */ 849, 837, -1,
  /* DiagSubGroup840 */ 842, 841, 845, 846, -1,
  /* DiagSubGroup849 */ 832, 836, 834, 835, -1,
  /* DiagSubGroup868 */ 671, -1,
  /* DiagSubGroup869 */ 870, -1,
  /* DiagSubGroup875 */ 777, 788, 876, -1,
  /* DiagSubGroup886 */ 135, -1,
  /* DiagSubGroup890 */ 895, 893, 894, -1,
  /* DiagSubGroup891 */ 890, 892, 896, -1,
  /* DiagSubGroup911 */ 912, 919, 921, 928, 922, 923, 932, 933, 914, 929, -1,
  /* DiagSubGroup919 */ 887, -1,
  /* DiagSubGroup924 */ 923, -1,
  /* DiagSubGroup926 */ 888, -1,
  /* DiagSubGroup931 */ 887, -1,
  /* DiagSubGroup932 */ 916, 930, 868, -1,
  /* DiagSubGroup933 */ 917, -1,
  /* DiagSubGroup942 */ 941, -1,
  /* DiagSubGroup945 */ 946, -1,
  /* DiagSubGroup948 */ 947, -1,
  /* DiagSubGroup954 */ 238, -1,
  /* DiagSubGroup955 */ 954, -1,
};

static const char DiagGroupNames[] = {
    "\000\020#pragma-messages\t#warnings\020CFString-literal\003CL4\032Indep"
    "endentClass-attribute\022NSObject-attribute\003abi\016absolute-value\024"
    "abstract-final-class\023abstract-vbase-init\007address\030address-of-pa"
    "cked-member\024address-of-temporary\020aggregate-return\naix-compat\016"
    "align-mismatch\003all\006alloca\031alloca-with-align-alignof\027always-"
    "inline-coroutine\020ambiguous-delete\022ambiguous-ellipsis\017ambiguous"
    "-macro\031ambiguous-member-template\033ambiguous-reversed-operator\034a"
    "nalyzer-incompatible-plugin\031anon-enum-enum-conversion\025anonymous-p"
    "ack-parens\003arc%arc-bridge-casts-disallowed-in-nonarc\036arc-maybe-re"
    "peated-use-of-weak\025arc-non-pod-memaccess\031arc-performSelector-leak"
    "s\030arc-repeated-use-of-weak\021arc-retain-cycles\032arc-unsafe-retain"
    "ed-assign\026argument-outside-range\034argument-undefined-behaviour\014"
    "array-bounds\037array-bounds-pointer-arithmetic\017array-parameter\003a"
    "sm\022asm-operand-widths\013assign-enum\006assume\013at-protocol\034ati"
    "mport-in-framework-header\015atomic-access\020atomic-alignment\027atomi"
    "c-implicit-seq-cst\026atomic-memory-ordering\021atomic-properties*atomi"
    "c-property-with-user-defined-accessor\035attribute-packed-for-bitfield\021"
    "attribute-warning\nattributes\033auto-disable-vptr-sanitizer\013auto-im"
    "port\022auto-storage-class\013auto-var-id\014availability\030avr-rtlib-"
    "linking-quirks\016backend-plugin\030backslash-newline-escape\021bad-fun"
    "ction-cast\016binary-literal\026bind-to-temporary-copy\024binding-in-co"
    "ndition\021bit-int-extension\034bitfield-constant-conversion\030bitfiel"
    "d-enum-conversion\016bitfield-width\037bitwise-conditional-parentheses\032"
    "bitwise-instead-of-logical\026bitwise-op-parentheses\033block-capture-a"
    "utoreleasing\017bool-conversion\020bool-conversions\016bool-operation\022"
    "braced-scalar-init\021branch-protection\013bridge-cast builtin-assume-a"
    "ligned-alignment\027builtin-macro-redefined\027builtin-memcpy-chk-size\027"
    "builtin-requires-header\nc++-compat\014c++0x-compat\020c++0x-extensions"
    "\017c++0x-narrowing\014c++11-compat(c++11-compat-deprecated-writable-st"
    "rings\025c++11-compat-pedantic*c++11-compat-reserved-user-defined-liter"
    "al\020c++11-extensions\020c++11-extra-semi\026c++11-inline-namespace\017"
    "c++11-long-long\017c++11-narrowing\032c++14-attribute-extensions\024c++"
    "14-binary-literal\014c++14-compat\025c++14-compat-pedantic\020c++14-ext"
    "ensions\032c++17-attribute-extensions\014c++17-compat\025c++17-compat-m"
    "angling\025c++17-compat-pedantic\020c++17-extensions\020c++1y-extension"
    "s\014c++1z-compat\025c++1z-compat-mangling\020c++1z-extensions\032c++20"
    "-attribute-extensions\014c++20-compat\025c++20-compat-pedantic\020c++20"
    "-designator\020c++20-extensions\014c++2a-compat\025c++2a-compat-pedanti"
    "c\020c++2a-extensions\020c++2b-extensions\036c++98-c++11-c++14-c++17-co"
    "mpat'c++98-c++11-c++14-c++17-compat-pedantic\030c++98-c++11-c++14-compa"
    "t!c++98-c++11-c++14-compat-pedantic\022c++98-c++11-compat!c++98-c++11-c"
    "ompat-binary-literal\033c++98-c++11-compat-pedantic\014c++98-compat#c++"
    "98-compat-bind-to-temporary-copy\027c++98-compat-extra-semi%c++98-compa"
    "t-local-type-template-args\025c++98-compat-pedantic'c++98-compat-unname"
    "d-type-template-args\016c11-extensions\nc2x-compat\016c2x-extensions\nc"
    "99-compat\016c99-designator\016c99-extensions#call-to-pure-virtual-from"
    "-ctor-dtor\025called-once-parameter\ncast-align\027cast-calling-convent"
    "ion\022cast-function-type\031cast-function-type-strict\020cast-of-sel-t"
    "ype\tcast-qual\023cast-qual-unrelated\nchar-align\017char-subscripts\014"
    "clang-cl-pch\020class-conversion\015class-varargs\017cmse-union-leak\005"
    "comma\007comment\010comments\036compare-distinct-pointer-types\022compl"
    "etion-handler\026complex-component-init\024compound-token-split\035comp"
    "ound-token-split-by-macro\035compound-token-split-by-space\031condition"
    "al-type-mismatch\031conditional-uninitialized\015config-macros\023const"
    "ant-conversion\022constant-evaluated\030constant-logical-operand\023con"
    "stexpr-not-const\010consumed\nconversion\017conversion-null$coro-non-al"
    "igned-allocation-funciton\tcoroutine%coroutine-missing-unhandled-except"
    "ion\026covered-switch-default\003cpp\030cstring-format-directive\026cta"
    "d-maybe-unsupported\021ctor-dtor-privacy\003ctu\013cuda-compat\030custo"
    "m-atomic-properties\027cxx-attribute-extension\010dangling\015dangling-"
    "else\016dangling-field\014dangling-gsl\031dangling-initializer-list\023"
    "darwin-sdk-settings\tdate-time\023dealloc-in-category\035debug-compress"
    "ion-unavailable\033declaration-after-statement\032defaulted-function-de"
    "leted\026delegating-ctor-cycles delete-abstract-non-virtual-dtor\021del"
    "ete-incomplete$delete-non-abstract-non-virtual-dtor\027delete-non-virtu"
    "al-dtor#delimited-escape-sequence-extension\032deprecate-lax-vec-conv-a"
    "ll\ndeprecated\035deprecated-altivec-src-compat$deprecated-anon-enum-en"
    "um-conversion\030deprecated-array-compare\025deprecated-attributes\023d"
    "eprecated-builtins\032deprecated-comma-subscript\017deprecated-copy\024"
    "deprecated-copy-dtor\031deprecated-copy-with-dtor'deprecated-copy-with-"
    "user-provided-copy'deprecated-copy-with-user-provided-dtor\024deprecate"
    "d-coroutine\027deprecated-declarations!deprecated-dynamic-exception-spe"
    "c\027deprecated-enum-compare#deprecated-enum-compare-conditional\037dep"
    "recated-enum-enum-conversion deprecated-enum-float-conversion!deprecate"
    "d-experimental-coroutine\032deprecated-implementations\031deprecated-in"
    "crement-bool\030deprecated-non-prototype\031deprecated-objc-isa-usage%d"
    "eprecated-objc-pointer-introspection5deprecated-objc-pointer-introspect"
    "ion-performSelector\021deprecated-pragma\023deprecated-register\037depr"
    "ecated-static-analyzer-flag\027deprecated-this-capture\017deprecated-ty"
    "pe\023deprecated-volatile\033deprecated-writable-strings\022direct-ivar"
    "-access\030disabled-macro-expansion\025disabled-optimization\014discard"
    "-qual\034distributed-object-modifiers\013div-by-zero\020division-by-zer"
    "o\036dll-attribute-on-redeclaration%dllexport-explicit-instantiation-de"
    "cl\032dllimport-static-field-def\015documentation\035documentation-depr"
    "ecated-sync\022documentation-html\026documentation-pedantic\035document"
    "ation-unknown-command\036dollar-in-identifier-extension\020double-promo"
    "tion\tdtor-name\014dtor-typedef\030duplicate-decl-specifier\016duplicat"
    "e-enum\024duplicate-method-arg\026duplicate-method-match\022duplicate-p"
    "rotocol\027dynamic-class-memaccess\026dynamic-exception-spec\006effc++\024"
    "elaborated-enum-base\025elaborated-enum-class\022embedded-directive\nem"
    "pty-body\023empty-decomposition\017empty-init-stmt\026empty-translation"
    "-unit\013encode-type\014endif-labels\014enum-compare\030enum-compare-co"
    "nditional\023enum-compare-switch\031enum-constexpr-conversion\017enum-c"
    "onversion\024enum-enum-conversion\025enum-float-conversion\016enum-too-"
    "large\nexceptions\023excess-initializers\025exit-time-destructors\024ex"
    "pansion-to-defined\030explicit-initialize-call\027explicit-ownership-ty"
    "pe\016export-unnamed\026export-using-directive\017extern-c-compat\022ex"
    "tern-initializer\005extra\023extra-qualification\nextra-semi\017extra-s"
    "emi-stmt\014extra-tokens\032final-dtor-non-final-class\013final-macro\024"
    "fixed-enum-extension\024fixed-point-overflow\tflag-enum\031flexible-arr"
    "ay-extensions\020float-conversion\013float-equal\031float-overflow-conv"
    "ersion\025float-zero-conversion\021for-loop-analysis\006format\021forma"
    "t-extra-args\030format-insufficient-args\030format-invalid-specifier\016"
    "format-non-iso\021format-nonliteral\017format-pedantic\017format-securi"
    "ty\025format-type-confusion\nformat-y2k\022format-zero-length\010format"
    "=2\016fortify-source\023four-char-constants\015frame-address\021frame-l"
    "arger-than\022frame-larger-than=%framework-include-private-from-public\023"
    "free-nonheap-object\036function-def-in-objc-container\025function-multi"
    "version\014fuse-ld-path\033future-attribute-extensions\015future-compat"
    "\ngcc-compat\023global-constructors\013global-isel\003gnu\026gnu-aligno"
    "f-expression\024gnu-anonymous-struct\033gnu-array-member-paren-init\015"
    "gnu-auto-type\022gnu-binary-literal\016gnu-case-range\023gnu-complex-in"
    "teger gnu-compound-literal-initializer\037gnu-conditional-omitted-opera"
    "nd\016gnu-designator\025gnu-empty-initializer\020gnu-empty-struct\036gn"
    "u-flexible-array-initializer\037gnu-flexible-array-union-member\024gnu-"
    "folding-constant\026gnu-imaginary-constant\020gnu-include-next\035gnu-i"
    "nline-cpp-without-extern\022gnu-label-as-value\017gnu-line-marker\033gn"
    "u-null-pointer-arithmetic\021gnu-pointer-arith\023gnu-redeclared-enum\030"
    "gnu-statement-expression-gnu-statement-expression-from-macro-expansion\025"
    "gnu-static-float-init$gnu-string-literal-operator-template\016gnu-union"
    "-cast\"gnu-variable-sized-type-not-at-end\027gnu-zero-line-directive!gn"
    "u-zero-variadic-macro-arguments\024gpu-maybe-wrong-side\014header-guard"
    "\016header-hygiene\010hip-only\017hlsl-extensions\025idiomatic-parenthe"
    "ses\022ignored-attributes)ignored-availability-without-sdk-settings\035"
    "ignored-optimization-argument\030ignored-pragma-intrinsic\027ignored-pr"
    "agma-optimize\017ignored-pragmas\022ignored-qualifiers\034ignored-refer"
    "ence-qualifiers\010implicit\032implicit-atomic-properties#implicit-cons"
    "t-int-float-conversion*implicit-conversion-floating-point-to-bool impli"
    "cit-exception-spec-mismatch\024implicit-fallthrough!implicit-fallthroug"
    "h-per-function\037implicit-fixed-point-conversion\031implicit-float-con"
    "version\035implicit-function-declaration\014implicit-int\027implicit-in"
    "t-conversion\035implicit-int-float-conversion\024implicit-retain-self\033"
    "implicitly-unsigned-literal\006import&import-preprocessor-directive-ped"
    "antic\021inaccessible-base\032include-next-absolute-path\033include-nex"
    "t-outside-header\033incompatible-exception-spec#incompatible-function-p"
    "ointer-types*incompatible-function-pointer-types-strict\"incompatible-l"
    "ibrary-redeclaration\026incompatible-ms-struct\032incompatible-pointer-"
    "types.incompatible-pointer-types-discards-qualifiers\032incompatible-pr"
    "operty-type\024incompatible-sysroot'incomplete-framework-module-declara"
    "tion\031incomplete-implementation\021incomplete-module\035incomplete-se"
    "tjmp-declaration\023incomplete-umbrella\026inconsistent-dllimport(incon"
    "sistent-missing-destructor-override\035inconsistent-missing-override\016"
    "increment-bool\022infinite-recursion\tinit-self\025initializer-override"
    "s\023injected-class-name\006inline\ninline-asm#inline-namespace-reopene"
    "d-noninline\021inline-new-delete\"instantiation-after-specialization\016"
    "int-conversion\017int-conversions\023int-in-bool-context\023int-to-poin"
    "ter-cast\030int-to-void-pointer-cast\020integer-overflow\031interrupt-s"
    "ervice-routine\035invalid-command-line-argument\021invalid-constexpr\020"
    "invalid-iboutlet&invalid-initializer-from-system-header\035invalid-ios-"
    "deployment-target\030invalid-no-builtin-names\020invalid-noreturn\020in"
    "valid-offsetof invalid-or-nonexistent-directory\036invalid-partial-spec"
    "ialization\013invalid-pch\020invalid-pp-token\027invalid-source-encodin"
    "g\023invalid-token-paste\014invalid-utf8\020jump-seh-finally\016keyword"
    "-compat\015keyword-macro\026knr-promoted-parameter\030language-extensio"
    "n-token\023large-by-value-copy\006liblto\017linker-warnings\022literal-"
    "conversion\015literal-range\030local-type-template-args\027logical-not-"
    "parentheses\026logical-op-parentheses\tlong-long\015loop-analysis\017ma"
    "cro-redefined\004main\020main-return-type\027malformed-warning-check\036"
    "many-braces-around-scalar-init*mathematical-notation-identifier-extensi"
    "on\nmax-tokens\021max-unsigned-zero\026memset-transposed-args\022memsiz"
    "e-comparison\021method-signatures\tmicrosoft\022microsoft-abstract\022m"
    "icrosoft-anon-tag\016microsoft-cast\021microsoft-charize\027microsoft-c"
    "omment-paste\024microsoft-const-init\023microsoft-cpp-macro\"microsoft-"
    "default-arg-redefinition\031microsoft-drectve-section\025microsoft-end-"
    "of-file microsoft-enum-forward-reference\024microsoft-enum-value\030mic"
    "rosoft-exception-spec\020microsoft-exists#microsoft-explicit-constructo"
    "r-call\035microsoft-extra-qualification\024microsoft-fixed-enum\030micr"
    "osoft-flexible-array\016microsoft-goto\033microsoft-inaccessible-base\021"
    "microsoft-include\033microsoft-mutable-reference\031microsoft-pure-defi"
    "nition\032microsoft-redeclare-static\020microsoft-sealed\027microsoft-s"
    "tatic-assert\022microsoft-template\031microsoft-template-shadow microso"
    "ft-union-member-reference\034microsoft-unqualified-friend\024microsoft-"
    "using-decl\032microsoft-void-pseudo-dtor\tmisexpect\026misleading-inden"
    "tation\025mismatched-new-delete\032mismatched-parameter-types\027mismat"
    "ched-return-types\017mismatched-tags\016missing-braces\021missing-const"
    "init\024missing-declarations\026missing-exception-spec\032missing-field"
    "-initializers\030missing-format-attribute\024missing-include-dirs\032mi"
    "ssing-method-return-type\020missing-noescape\020missing-noreturn\030mis"
    "sing-prototype-for-cc\022missing-prototypes\025missing-selector-name\017"
    "missing-sysroot\035missing-variable-declarations\025misspelled-assumpti"
    "on\014module-build\017module-conflict\033module-file-config-mismatch\025"
    "module-file-extension\015module-import\031module-import-in-extern-c\032"
    "module-include-translation\013module-lock\"modules-ambiguous-internal-l"
    "inkage\037modules-import-nested-redundant\004most\004move\014msvc-inclu"
    "de\016msvc-not-found\tmultichar\023multiple-move-vbase\tnarrowing\021ne"
    "sted-anon-types\016nested-externs\020new-returns-null\013newline-eof\007"
    "noderef\015noexcept-type\031non-c-typedef-for-linkage\007non-gcc\033non"
    "-literal-null-conversion'non-modular-include-in-framework-module\035non"
    "-modular-include-in-module\017non-pod-varargs\032non-power-of-two-align"
    "ment\020non-virtual-dtor\007nonnull\025nonportable-cfstrings\030nonport"
    "able-include-path\037nonportable-system-include-path!nonportable-vector"
    "-initialization\024nontrivial-memaccess\023nsconsumed-mismatch\022nsret"
    "urns-mismatch\017null-arithmetic\016null-character\017null-conversion\020"
    "null-dereference\027null-pointer-arithmetic\030null-pointer-subtraction"
    "\013nullability\030nullability-completeness\"nullability-completeness-o"
    "n-arrays\024nullability-declspec\025nullability-extension#nullability-i"
    "nferred-on-nested-type\036nullable-to-nonnull-conversion+objc-autosynth"
    "esis-property-ivar-name-match\035objc-bool-constant-conversion\013objc-"
    "boxing\027objc-circular-container\016objc-cocoa-api\034objc-designated-"
    "initializers\036objc-dictionary-duplicate-keys\023objc-flexible-array\037"
    "objc-forward-class-redefinition\024objc-interface-ivars\024objc-literal"
    "-compare\027objc-literal-conversion\027objc-macro-redefinition\021objc-"
    "messaging-id\022objc-method-access\037objc-missing-property-synthesis\030"
    "objc-missing-super-calls\032objc-multiple-method-names\"objc-noncopy-re"
    "tain-block-property\032objc-nonunified-exceptions#objc-property-assign-"
    "on-object-type\034objc-property-implementation\037objc-property-implici"
    "t-mismatch*objc-property-matches-cocoa-ownership-rule\032objc-property-"
    "no-attribute\027objc-property-synthesis#objc-protocol-method-implementa"
    "tion objc-protocol-property-synthesis\030objc-protocol-qualifiers\"objc"
    "-readonly-with-setter-property\026objc-redundant-api-use\032objc-redund"
    "ant-literal-use\017objc-root-class\025objc-signed-char-bool/objc-signed"
    "-char-bool-implicit-float-conversion-objc-signed-char-bool-implicit-int"
    "-conversion\023objc-string-compare\031objc-string-concatenation\034objc"
    "-unsafe-perform-selector\003odr\016old-style-cast\024old-style-definiti"
    "on\027opencl-unsupported-rgba\006openmp\024openmp-51-extensions\016open"
    "mp-clauses\020openmp-loop-form\016openmp-mapping\015openmp-target\016op"
    "tion-ignored!ordered-compare-function-pointers\027out-of-line-declarati"
    "on\025out-of-scope-function\014over-aligned\010overflow\022overlength-s"
    "trings\037overloaded-shift-op-parentheses\022overloaded-virtual\015over"
    "ride-init\017override-module\032overriding-method-mismatch\023overridin"
    "g-t-option\006packed\016packed-non-pod\006padded\013parentheses\024pare"
    "ntheses-equality\024partial-availability\004pass\015pass-analysis\013pa"
    "ss-failed\013pass-missed\015pch-date-time\010pedantic\026pedantic-core-"
    "features\017pedantic-macros\020pessimizing-move\015pointer-arith\027poi"
    "nter-bool-conversion\017pointer-compare\027pointer-integer-compare\014p"
    "ointer-sign\024pointer-to-enum-cast\023pointer-to-int-cast\025pointer-t"
    "ype-mismatch\031poison-system-directories\033potentially-direct-selecto"
    "r potentially-evaluated-expression\026pragma-clang-attribute\032pragma-"
    "once-outside-header\013pragma-pack\036pragma-pack-suspicious-include#pr"
    "agma-system-header-outside-header\007pragmas\020pre-c++14-compat\031pre"
    "-c++14-compat-pedantic\020pre-c++17-compat\031pre-c++17-compat-pedantic"
    "\020pre-c++20-compat\031pre-c++20-compat-pedantic\020pre-c++2b-compat\031"
    "pre-c++2b-compat-pedantic\016pre-c2x-compat\027pre-c2x-compat-pedantic\024"
    "pre-openmp-51-compat&predefined-identifier-outside-function\016private-"
    "extern\016private-header\016private-module\025profile-instr-missing\031"
    "profile-instr-out-of-date\030profile-instr-unprofiled\032property-acces"
    "s-dot-syntax\033property-attribute-mismatch\010protocol%protocol-proper"
    "ty-synthesis-ambiguity\005psabi\032qualified-void-return-type\"quoted-i"
    "nclude-in-framework-header\023range-loop-analysis\031range-loop-bind-re"
    "ference\024range-loop-construct\017read-only-types\032readonly-iboutlet"
    "-property\015receiver-expr\026receiver-forward-class\027redeclared-clas"
    "s-member\026redundant-consteval-if\017redundant-decls\016redundant-move"
    "\020redundant-parens\010register\026reinterpret-base-class\025remark-ba"
    "ckend-plugin\007reorder\014reorder-ctor\021reorder-init-list\030require"
    "s-super-attribute\021reserved-id-macro\023reserved-identifier\031reserv"
    "ed-macro-identifier\035reserved-user-defined-literal\022restrict-expans"
    "ion\031retained-language-linkage\024return-stack-address\017return-std-"
    "move\013return-type\025return-type-c-linkage\020rewrite-not-bool\023rou"
    "nd-trip-cc1-args\004rtti\020sanitize-address\025sarif-format-unstable\021"
    "search-path-usage\007section\010selector\026selector-type-mismatch\013s"
    "elf-assign\021self-assign-field\026self-assign-overloaded\tself-move\034"
    "semicolon-before-method-body\010sentinel\016sequence-point\026serialize"
    "d-diagnostics\006shadow\nshadow-all\014shadow-field\033shadow-field-in-"
    "constructor$shadow-field-in-constructor-modified\013shadow-ivar\027shad"
    "ow-uncaptured-local\024shift-count-negative\024shift-count-overflow\024"
    "shift-negative-value\024shift-op-parentheses\016shift-overflow\023shift"
    "-sign-overflow\020shorten-64-to-32\014sign-compare\017sign-conversion\n"
    "sign-promo\024signed-enum-bitfield\025signed-unsigned-wchar'single-bit-"
    "bitfield-constant-conversion\025sizeof-array-argument\022sizeof-array-d"
    "ecay\020sizeof-array-div\022sizeof-pointer-div\030sizeof-pointer-memacc"
    "ess\020slash-u-filename\014slh-asm-goto\nsloc-usage\027sometimes-uninit"
    "ialized\nsource-mgr\022source-uses-openmp\013spir-compat\014spirv-compa"
    "t\017stack-exhausted\017stack-protector\021static-float-init\020static-"
    "in-inline$static-inline-explicit-instantiation\026static-local-in-inlin"
    "e\020static-self-init\023stdlibcxx-not-found\017strict-aliasing\021stri"
    "ct-aliasing=0\021strict-aliasing=1\021strict-aliasing=2\017strict-overf"
    "low\021strict-overflow=0\021strict-overflow=1\021strict-overflow=2\021s"
    "trict-overflow=3\021strict-overflow=4\021strict-overflow=5\"strict-pote"
    "ntially-direct-selector\021strict-prototypes\025strict-selector-match\016"
    "string-compare\024string-concatenation\021string-conversion\020string-p"
    "lus-char\017string-plus-int\024strlcpy-strlcat-size\014strncat-size\033"
    "suggest-destructor-override\020suggest-override\033super-class-method-m"
    "ismatch\020suspicious-bzero\024suspicious-memaccess\024swift-name-attri"
    "bute\006switch\013switch-bool\016switch-default\013switch-enum%sync-fet"
    "ch-and-nand-semantics-changed\005synth\036target-clones-mixed-specifier"
    "s\034tautological-bitwise-compare\024tautological-compare\035tautologic"
    "al-constant-compare&tautological-constant-in-range-compare*tautological"
    "-constant-out-of-range-compare\036tautological-objc-bool-compare\034tau"
    "tological-overlap-compare\034tautological-pointer-compare\037tautologic"
    "al-type-limit-compare\036tautological-undefined-compare'tautological-un"
    "signed-char-zero-compare'tautological-unsigned-enum-zero-compare\"tauto"
    "logical-unsigned-zero-compare tautological-value-range-compare\017tcb-e"
    "nforcement$tentative-definition-incomplete-type\015thread-safety\026thr"
    "ead-safety-analysis\030thread-safety-attributes\022thread-safety-beta\026"
    "thread-safety-negative\025thread-safety-precise\027thread-safety-refere"
    "nce\025thread-safety-verbose\ttrigraphs\013type-limits\013type-safety\024"
    "typedef-redefinition\020typename-missing\031unable-to-open-stats-file\020"
    "unaligned-access!unaligned-qualifier-implicit-cast\030unavailable-decla"
    "rations\023undeclared-selector\005undef\014undef-prefix\031undefined-bo"
    "ol-conversion\027undefined-func-template\020undefined-inline\022undefin"
    "ed-internal\027undefined-internal-type\032undefined-reinterpret-cast\026"
    "undefined-var-template\035underaligned-exception-object\026unevaluated-"
    "expression\026unguarded-availability\032unguarded-availability-new\007u"
    "nicode\021unicode-homoglyph\022unicode-whitespace\022unicode-zero-width"
    "\015uninitialized\035uninitialized-const-reference\020unknown-argument\022"
    "unknown-assumption\022unknown-attributes\024unknown-cuda-version\022unk"
    "nown-directives\027unknown-escape-sequence\017unknown-pragmas\022unknow"
    "n-sanitizers\026unknown-warning-option\032unnamed-type-template-args\035"
    "unneeded-internal-declaration\030unneeded-member-function\031unqualifie"
    "d-std-cast-call\020unreachable-code\033unreachable-code-aggressive\026u"
    "nreachable-code-break\034unreachable-code-fallthrough\036unreachable-co"
    "de-generic-assoc\037unreachable-code-loop-increment\027unreachable-code"
    "-return\023unsafe-buffer-usage\013unsequenced\017unsupported-abi\017uns"
    "upported-abs\036unsupported-availability-guard\016unsupported-cb#unsupp"
    "orted-dll-base-class-template\036unsupported-floating-point-opt\022unsu"
    "pported-friend\021unsupported-gpopt\017unsupported-nan\026unsupported-t"
    "arget-opt\026unsupported-visibility\037unusable-partial-specialization\006"
    "unused\017unused-argument\030unused-but-set-parameter\027unused-but-set"
    "-variable\034unused-command-line-argument\021unused-comparison\025unuse"
    "d-const-variable\032unused-exception-parameter\017unused-function\032un"
    "used-getter-return-value\014unused-label\025unused-lambda-capture\024un"
    "used-local-typedef\025unused-local-typedefs\015unused-macros\026unused-"
    "member-function\020unused-parameter\024unused-private-field\024unused-p"
    "roperty-ivar\015unused-result\017unused-template\014unused-value\017unu"
    "sed-variable\026unused-volatile-lvalue\026used-but-marked-unused\025use"
    "r-defined-literals\025user-defined-warnings\007varargs\017variadic-macr"
    "os\015vec-elem-size\021vector-conversion\022vector-conversions\014vexin"
    "g-parse\nvisibility\003vla\015vla-extension\031void-pointer-to-enum-cas"
    "t\030void-pointer-to-int-cast\024void-ptr-dereference\025volatile-regis"
    "ter-var\023wasm-exception-spec\025weak-template-vtables\014weak-vtables"
    "\020writable-strings\015write-strings\017xor-used-as-pow\035zero-as-nul"
    "l-pointer-constant\021zero-length-array"};

#endif // GET_DIAG_ARRAYS


#ifdef DIAG_ENTRY
DIAG_ENTRY(anonymous_54 /*  */, 0, 0, /* DiagSubGroup0 */ 1, R"()")
DIAG_ENTRY(PoundPragmaMessage /* #pragma-messages */, 1, /* DiagArray1 */ 1, 0, R"()")
DIAG_ENTRY(PoundWarning /* #warnings */, 18, /* DiagArray2 */ 3, 0, R"()")
DIAG_ENTRY(anonymous_258 /* CFString-literal */, 28, /* DiagArray3 */ 5, 0, R"()")
DIAG_ENTRY(anonymous_53 /* CL4 */, 45, 0, /* DiagSubGroup4 */ 3, R"()")
DIAG_ENTRY(IndependentClassAttribute /* IndependentClass-attribute */, 49, /* DiagArray5 */ 7, 0, R"()")
DIAG_ENTRY(NSobjectAttribute /* NSObject-attribute */, 76, /* DiagArray6 */ 10, 0, R"()")
DIAG_ENTRY(anonymous_0 /* abi */, 95, 0, 0, R"()")
DIAG_ENTRY(AbsoluteValue /* absolute-value */, 99, /* DiagArray8 */ 12, 0, R"()")
DIAG_ENTRY(AbstractFinalClass /* abstract-final-class */, 114, /* DiagArray9 */ 17, 0, R"()")
DIAG_ENTRY(anonymous_252 /* abstract-vbase-init */, 135, /* DiagArray10 */ 19, 0, R"()")
DIAG_ENTRY(anonymous_52 /* address */, 155, 0, /* DiagSubGroup11 */ 6, R"()")
DIAG_ENTRY(anonymous_230 /* address-of-packed-member */, 163, /* DiagArray12 */ 21, 0, R"()")
DIAG_ENTRY(AddressOfTemporary /* address-of-temporary */, 188, /* DiagArray13 */ 23, 0, R"()")
DIAG_ENTRY(anonymous_1 /* aggregate-return */, 209, 0, 0, R"()")
DIAG_ENTRY(AIXCompat /* aix-compat */, 226, /* DiagArray15 */ 25, 0, R"()")
DIAG_ENTRY(anonymous_231 /* align-mismatch */, 237, /* DiagArray16 */ 28, 0, R"()")
DIAG_ENTRY(All /* all */, 252, 0, /* DiagSubGroup17 */ 10, R"()")
DIAG_ENTRY(anonymous_176 /* alloca */, 256, /* DiagArray18 */ 30, 0, R"()")
DIAG_ENTRY(anonymous_177 /* alloca-with-align-alignof */, 263, /* DiagArray19 */ 32, 0, R"()")
DIAG_ENTRY(AlwaysInlineCoroutine /* always-inline-coroutine */, 289, /* DiagArray20 */ 34, 0, R"()")
DIAG_ENTRY(anonymous_237 /* ambiguous-delete */, 313, /* DiagArray21 */ 36, 0, R"()")
DIAG_ENTRY(anonymous_132 /* ambiguous-ellipsis */, 330, /* DiagArray22 */ 38, 0, R"()")
DIAG_ENTRY(AmbiguousMacro /* ambiguous-macro */, 349, /* DiagArray23 */ 40, 0, R"()")
DIAG_ENTRY(AmbigMemberTemplate /* ambiguous-member-template */, 365, /* DiagArray24 */ 42, 0, R"()")
DIAG_ENTRY(anonymous_197 /* ambiguous-reversed-operator */, 391, /* DiagArray25 */ 44, 0, R"()")
DIAG_ENTRY(anonymous_91 /* analyzer-incompatible-plugin */, 419, /* DiagArray26 */ 46, 0, R"()")
DIAG_ENTRY(AnonEnumEnumConversion /* anon-enum-enum-conversion */, 448, /* DiagArray27 */ 48, /* DiagSubGroup27 */ 16, R"()")
DIAG_ENTRY(anonymous_133 /* anonymous-pack-parens */, 474, /* DiagArray28 */ 50, 0, R"()")
DIAG_ENTRY(AutomaticReferenceCounting /* arc */, 496, 0, /* DiagSubGroup29 */ 18, R"()")
DIAG_ENTRY(anonymous_131 /* arc-bridge-casts-disallowed-in-nonarc */, 500, /* DiagArray30 */ 52, 0, R"()")
DIAG_ENTRY(ARCRepeatedUseOfWeakMaybe /* arc-maybe-repeated-use-of-weak */, 538, /* DiagArray31 */ 54, 0, R"()")
DIAG_ENTRY(ARCNonPodMemAccess /* arc-non-pod-memaccess */, 569, /* DiagArray32 */ 56, 0, R"()")
DIAG_ENTRY(anonymous_161 /* arc-performSelector-leaks */, 591, /* DiagArray33 */ 58, 0, R"()")
DIAG_ENTRY(ARCRepeatedUseOfWeak /* arc-repeated-use-of-weak */, 617, /* DiagArray34 */ 60, /* DiagSubGroup34 */ 22, R"()")
DIAG_ENTRY(ARCRetainCycles /* arc-retain-cycles */, 642, /* DiagArray35 */ 62, 0, R"()")
DIAG_ENTRY(ARCUnsafeRetainedAssign /* arc-unsafe-retained-assign */, 660, /* DiagArray36 */ 64, 0, R"()")
DIAG_ENTRY(anonymous_260 /* argument-outside-range */, 687, /* DiagArray37 */ 68, 0, R"()")
DIAG_ENTRY(anonymous_261 /* argument-undefined-behaviour */, 710, /* DiagArray38 */ 70, 0, R"()")
DIAG_ENTRY(ArrayBounds /* array-bounds */, 739, /* DiagArray39 */ 72, 0, R"()")
DIAG_ENTRY(ArrayBoundsPointerArithmetic /* array-bounds-pointer-arithmetic */, 752, /* DiagArray40 */ 78, 0, R"()")
DIAG_ENTRY(ArrayParameter /* array-parameter */, 784, /* DiagArray41 */ 81, 0, R"()")
DIAG_ENTRY(ASM /* asm */, 800, 0, /* DiagSubGroup42 */ 24, R"()")
DIAG_ENTRY(ASMOperandWidths /* asm-operand-widths */, 804, /* DiagArray43 */ 83, 0, R"()")
DIAG_ENTRY(anonymous_259 /* assign-enum */, 823, /* DiagArray44 */ 85, 0, R"()")
DIAG_ENTRY(anonymous_148 /* assume */, 835, /* DiagArray45 */ 87, 0, R"()")
DIAG_ENTRY(anonymous_51 /* at-protocol */, 842, 0, 0, R"()")
DIAG_ENTRY(FrameworkHdrAtImport /* atimport-in-framework-header */, 854, /* DiagArray47 */ 89, 0, R"()")
DIAG_ENTRY(anonymous_219 /* atomic-access */, 883, /* DiagArray48 */ 91, 0, R"()")
DIAG_ENTRY(AtomicAlignment /* atomic-alignment */, 897, /* DiagArray49 */ 93, 0, R"()")
DIAG_ENTRY(anonymous_245 /* atomic-implicit-seq-cst */, 914, /* DiagArray50 */ 96, 0, R"()")
DIAG_ENTRY(anonymous_244 /* atomic-memory-ordering */, 938, /* DiagArray51 */ 98, 0, R"()")
DIAG_ENTRY(AtomicProperties /* atomic-properties */, 961, 0, /* DiagSubGroup52 */ 26, R"()")
DIAG_ENTRY(anonymous_156 /* atomic-property-with-user-defined-accessor */, 979, /* DiagArray53 */ 100, 0, R"()")
DIAG_ENTRY(anonymous_193 /* attribute-packed-for-bitfield */, 1022, /* DiagArray54 */ 102, 0, R"()")
DIAG_ENTRY(BackendWarningAttributes /* attribute-warning */, 1052, /* DiagArray55 */ 104, 0, R"()")
DIAG_ENTRY(Attributes /* attributes */, 1070, 0, /* DiagSubGroup56 */ 29, R"()")
DIAG_ENTRY(AutoDisableVptrSanitizer /* auto-disable-vptr-sanitizer */, 1081, /* DiagArray57 */ 106, 0, R"()")
DIAG_ENTRY(anonymous_2 /* auto-import */, 1109, 0, 0, R"()")
DIAG_ENTRY(anonymous_129 /* auto-storage-class */, 1121, /* DiagArray59 */ 108, 0, R"()")
DIAG_ENTRY(anonymous_173 /* auto-var-id */, 1140, /* DiagArray60 */ 110, 0, R"()")
DIAG_ENTRY(Availability /* availability */, 1152, /* DiagArray61 */ 112, 0, R"()")
DIAG_ENTRY(AVRRtlibLinkingQuirks /* avr-rtlib-linking-quirks */, 1165, /* DiagArray62 */ 123, 0, R"()")
DIAG_ENTRY(BackendPlugin /* backend-plugin */, 1190, /* DiagArray63 */ 129, 0, R"()")
DIAG_ENTRY(anonymous_94 /* backslash-newline-escape */, 1205, /* DiagArray64 */ 132, 0, R"()")
DIAG_ENTRY(BadFunctionCast /* bad-function-cast */, 1230, /* DiagArray65 */ 134, 0, R"()")
DIAG_ENTRY(BinaryLiteral /* binary-literal */, 1248, 0, /* DiagSubGroup66 */ 32, R"()")
DIAG_ENTRY(BindToTemporaryCopy /* bind-to-temporary-copy */, 1263, /* DiagArray67 */ 136, /* DiagSubGroup67 */ 36, R"()")
DIAG_ENTRY(anonymous_143 /* binding-in-condition */, 1286, /* DiagArray68 */ 139, 0, R"()")
DIAG_ENTRY(anonymous_136 /* bit-int-extension */, 1307, /* DiagArray69 */ 141, 0, R"()")
DIAG_ENTRY(BitFieldConstantConversion /* bitfield-constant-conversion */, 1325, /* DiagArray70 */ 143, /* DiagSubGroup70 */ 38, R"()")
DIAG_ENTRY(BitFieldEnumConversion /* bitfield-enum-conversion */, 1354, /* DiagArray71 */ 145, 0, R"()")
DIAG_ENTRY(BitFieldWidth /* bitfield-width */, 1379, /* DiagArray72 */ 149, 0, R"()")
DIAG_ENTRY(BitwiseConditionalParentheses /* bitwise-conditional-parentheses */, 1394, /* DiagArray73 */ 151, 0, R"()")
DIAG_ENTRY(BitwiseInsteadOfLogical /* bitwise-instead-of-logical */, 1426, /* DiagArray74 */ 153, 0, R"()")
DIAG_ENTRY(BitwiseOpParentheses /* bitwise-op-parentheses */, 1453, /* DiagArray75 */ 155, 0, R"()")
DIAG_ENTRY(BlockCaptureAutoReleasing /* block-capture-autoreleasing */, 1476, /* DiagArray76 */ 157, 0, R"()")
DIAG_ENTRY(BoolConversion /* bool-conversion */, 1504, /* DiagArray77 */ 159, /* DiagSubGroup77 */ 40, R"()")
DIAG_ENTRY(anonymous_59 /* bool-conversions */, 1520, 0, /* DiagSubGroup78 */ 43, R"()")
DIAG_ENTRY(BoolOperation /* bool-operation */, 1537, /* DiagArray79 */ 161, /* DiagSubGroup79 */ 45, R"()")
DIAG_ENTRY(anonymous_214 /* braced-scalar-init */, 1552, /* DiagArray80 */ 163, 0, R"()")
DIAG_ENTRY(BranchProtection /* branch-protection */, 1571, /* DiagArray81 */ 165, 0, R"()")
DIAG_ENTRY(ObjCBridge /* bridge-cast */, 1589, /* DiagArray82 */ 170, 0, R"()")
DIAG_ENTRY(anonymous_179 /* builtin-assume-aligned-alignment */, 1601, /* DiagArray83 */ 173, 0, R"()")
DIAG_ENTRY(BuiltinMacroRedefined /* builtin-macro-redefined */, 1634, /* DiagArray84 */ 175, 0, R"()")
DIAG_ENTRY(anonymous_149 /* builtin-memcpy-chk-size */, 1658, /* DiagArray85 */ 178, 0, R"()")
DIAG_ENTRY(BuiltinRequiresHeader /* builtin-requires-header */, 1682, /* DiagArray86 */ 180, 0, R"()")
DIAG_ENTRY(CXXCompat /* c++-compat */, 1706, /* DiagArray87 */ 182, 0, R"()")
DIAG_ENTRY(anonymous_19 /* c++0x-compat */, 1717, 0, /* DiagSubGroup88 */ 47, R"()")
DIAG_ENTRY(anonymous_63 /* c++0x-extensions */, 1730, 0, /* DiagSubGroup89 */ 49, R"()")
DIAG_ENTRY(anonymous_17 /* c++0x-narrowing */, 1747, 0, /* DiagSubGroup90 */ 51, R"()")
DIAG_ENTRY(CXX11Compat /* c++11-compat */, 1763, /* DiagArray91 */ 184, /* DiagSubGroup91 */ 53, R"()")
DIAG_ENTRY(CXX11CompatDeprecatedWritableStr /* c++11-compat-deprecated-writable-strings */, 1776, /* DiagArray92 */ 195, 0, R"()")
DIAG_ENTRY(CXX11CompatPedantic /* c++11-compat-pedantic */, 1817, 0, /* DiagSubGroup93 */ 61, R"()")
DIAG_ENTRY(CXX11CompatReservedUserDefinedLiteral /* c++11-compat-reserved-user-defined-literal */, 1839, /* DiagArray94 */ 197, 0, R"()")
DIAG_ENTRY(CXX11 /* c++11-extensions */, 1882, /* DiagArray95 */ 199, /* DiagSubGroup95 */ 67, R"()")
DIAG_ENTRY(CXX11ExtraSemi /* c++11-extra-semi */, 1899, /* DiagArray96 */ 225, 0, R"()")
DIAG_ENTRY(CXX11InlineNamespace /* c++11-inline-namespace */, 1916, /* DiagArray97 */ 227, 0, R"()")
DIAG_ENTRY(CXX11LongLong /* c++11-long-long */, 1939, /* DiagArray98 */ 229, 0, R"()")
DIAG_ENTRY(CXX11Narrowing /* c++11-narrowing */, 1955, /* DiagArray99 */ 231, 0, R"()")
DIAG_ENTRY(CXX14Attrs /* c++14-attribute-extensions */, 1971, /* DiagArray100 */ 239, 0, R"()")
DIAG_ENTRY(CXX14BinaryLiteral /* c++14-binary-literal */, 1998, /* DiagArray101 */ 241, 0, R"()")
DIAG_ENTRY(CXX14Compat /* c++14-compat */, 2019, 0, /* DiagSubGroup102 */ 71, R"()")
DIAG_ENTRY(CXX14CompatPedantic /* c++14-compat-pedantic */, 2032, 0, /* DiagSubGroup103 */ 75, R"()")
DIAG_ENTRY(CXX14 /* c++14-extensions */, 2054, /* DiagArray104 */ 243, /* DiagSubGroup104 */ 80, R"()")
DIAG_ENTRY(CXX17Attrs /* c++17-attribute-extensions */, 2071, /* DiagArray105 */ 251, 0, R"()")
DIAG_ENTRY(CXX17Compat /* c++17-compat */, 2098, 0, /* DiagSubGroup106 */ 83, R"()")
DIAG_ENTRY(CXX17CompatMangling /* c++17-compat-mangling */, 2111, /* DiagArray107 */ 253, 0, R"()")
DIAG_ENTRY(CXX17CompatPedantic /* c++17-compat-pedantic */, 2133, 0, /* DiagSubGroup108 */ 89, R"()")
DIAG_ENTRY(CXX17 /* c++17-extensions */, 2155, /* DiagArray109 */ 255, /* DiagSubGroup109 */ 93, R"()")
DIAG_ENTRY(anonymous_64 /* c++1y-extensions */, 2172, 0, /* DiagSubGroup110 */ 95, R"()")
DIAG_ENTRY(anonymous_20 /* c++1z-compat */, 2189, 0, /* DiagSubGroup111 */ 97, R"()")
DIAG_ENTRY(anonymous_10 /* c++1z-compat-mangling */, 2202, 0, /* DiagSubGroup112 */ 99, R"()")
DIAG_ENTRY(anonymous_65 /* c++1z-extensions */, 2224, 0, /* DiagSubGroup113 */ 101, R"()")
DIAG_ENTRY(CXX20Attrs /* c++20-attribute-extensions */, 2241, /* DiagArray114 */ 273, 0, R"()")
DIAG_ENTRY(CXX20Compat /* c++20-compat */, 2268, /* DiagArray115 */ 275, /* DiagSubGroup115 */ 103, R"()")
DIAG_ENTRY(CXX20CompatPedantic /* c++20-compat-pedantic */, 2281, 0, /* DiagSubGroup116 */ 105, R"()")
DIAG_ENTRY(CXX20Designator /* c++20-designator */, 2303, /* DiagArray117 */ 285, 0, R"()")
DIAG_ENTRY(CXX20 /* c++20-extensions */, 2320, /* DiagArray118 */ 287, /* DiagSubGroup118 */ 108, R"()")
DIAG_ENTRY(anonymous_21 /* c++2a-compat */, 2337, 0, /* DiagSubGroup119 */ 111, R"()")
DIAG_ENTRY(anonymous_22 /* c++2a-compat-pedantic */, 2350, 0, /* DiagSubGroup120 */ 113, R"()")
DIAG_ENTRY(anonymous_66 /* c++2a-extensions */, 2372, 0, /* DiagSubGroup121 */ 115, R"()")
DIAG_ENTRY(CXX2b /* c++2b-extensions */, 2389, /* DiagArray122 */ 308, 0, R"()")
DIAG_ENTRY(anonymous_15 /* c++98-c++11-c++14-c++17-compat */, 2406, 0, /* DiagSubGroup123 */ 117, R"()")
DIAG_ENTRY(anonymous_16 /* c++98-c++11-c++14-c++17-compat-pedantic */, 2437, 0, /* DiagSubGroup124 */ 119, R"()")
DIAG_ENTRY(anonymous_13 /* c++98-c++11-c++14-compat */, 2477, 0, /* DiagSubGroup125 */ 121, R"()")
DIAG_ENTRY(anonymous_14 /* c++98-c++11-c++14-compat-pedantic */, 2502, 0, /* DiagSubGroup126 */ 123, R"()")
DIAG_ENTRY(anonymous_11 /* c++98-c++11-compat */, 2536, 0, /* DiagSubGroup127 */ 125, R"()")
DIAG_ENTRY(CXXPre14CompatBinaryLiteral /* c++98-c++11-compat-binary-literal */, 2555, /* DiagArray128 */ 320, 0, R"()")
DIAG_ENTRY(anonymous_12 /* c++98-c++11-compat-pedantic */, 2589, 0, /* DiagSubGroup129 */ 127, R"()")
DIAG_ENTRY(CXX98Compat /* c++98-compat */, 2617, /* DiagArray130 */ 322, /* DiagSubGroup130 */ 129, R"()")
DIAG_ENTRY(CXX98CompatBindToTemporaryCopy /* c++98-compat-bind-to-temporary-copy */, 2630, /* DiagArray131 */ 382, 0, R"()")
DIAG_ENTRY(CXX98CompatExtraSemi /* c++98-compat-extra-semi */, 2666, /* DiagArray132 */ 384, 0, R"()")
DIAG_ENTRY(CXX98CompatLocalTypeTemplateArgs /* c++98-compat-local-type-template-args */, 2690, /* DiagArray133 */ 386, 0, R"()")
DIAG_ENTRY(CXX98CompatPedantic /* c++98-compat-pedantic */, 2728, /* DiagArray134 */ 388, /* DiagSubGroup134 */ 136, R"()")
DIAG_ENTRY(CXX98CompatUnnamedTypeTemplateArgs /* c++98-compat-unnamed-type-template-args */, 2750, /* DiagArray135 */ 398, 0, R"()")
DIAG_ENTRY(C11 /* c11-extensions */, 2790, /* DiagArray136 */ 400, 0, R"()")
DIAG_ENTRY(C2xCompat /* c2x-compat */, 2805, /* DiagArray137 */ 405, 0, R"()")
DIAG_ENTRY(C2x /* c2x-extensions */, 2816, /* DiagArray138 */ 407, 0, R"()")
DIAG_ENTRY(C99Compat /* c99-compat */, 2831, /* DiagArray139 */ 414, 0, R"()")
DIAG_ENTRY(C99Designator /* c99-designator */, 2842, /* DiagArray140 */ 419, /* DiagSubGroup140 */ 144, R"()")
DIAG_ENTRY(C99 /* c99-extensions */, 2857, /* DiagArray141 */ 425, /* DiagSubGroup141 */ 146, R"()")
DIAG_ENTRY(PureVirtualCallFromCtorDtor /* call-to-pure-virtual-from-ctor-dtor */, 2872, /* DiagArray142 */ 436, 0, R"()")
DIAG_ENTRY(CalledOnceParameter /* called-once-parameter */, 2908, /* DiagArray143 */ 438, /* DiagSubGroup143 */ 148, R"()")
DIAG_ENTRY(CastAlign /* cast-align */, 2930, /* DiagArray144 */ 442, 0, R"()")
DIAG_ENTRY(anonymous_247 /* cast-calling-convention */, 2941, /* DiagArray145 */ 444, 0, R"()")
DIAG_ENTRY(CastFunctionType /* cast-function-type */, 2965, /* DiagArray146 */ 446, /* DiagSubGroup146 */ 150, R"()")
DIAG_ENTRY(CastFunctionTypeStrict /* cast-function-type-strict */, 2984, /* DiagArray147 */ 448, 0, R"()")
DIAG_ENTRY(SelTypeCast /* cast-of-sel-type */, 3010, /* DiagArray148 */ 450, 0, R"()")
DIAG_ENTRY(CastQual /* cast-qual */, 3027, /* DiagArray149 */ 452, 0, R"()")
DIAG_ENTRY(anonymous_235 /* cast-qual-unrelated */, 3037, /* DiagArray150 */ 455, 0, R"()")
DIAG_ENTRY(anonymous_3 /* char-align */, 3057, 0, 0, R"()")
DIAG_ENTRY(CharSubscript /* char-subscripts */, 3068, /* DiagArray152 */ 457, 0, R"()")
DIAG_ENTRY(ClangClPch /* clang-cl-pch */, 3084, /* DiagArray153 */ 460, 0, R"()")
DIAG_ENTRY(ClassConversion /* class-conversion */, 3097, /* DiagArray154 */ 465, 0, R"()")
DIAG_ENTRY(ClassVarargs /* class-varargs */, 3114, /* DiagArray155 */ 469, /* DiagSubGroup155 */ 152, R"()")
DIAG_ENTRY(anonymous_180 /* cmse-union-leak */, 3128, /* DiagArray156 */ 471, 0, R"()")
DIAG_ENTRY(anonymous_138 /* comma */, 3144, /* DiagArray157 */ 473, 0, R"()")
DIAG_ENTRY(Comment /* comment */, 3150, /* DiagArray158 */ 475, 0, R"()")
DIAG_ENTRY(anonymous_57 /* comments */, 3158, 0, /* DiagSubGroup159 */ 154, R"()")
DIAG_ENTRY(CompareDistinctPointerType /* compare-distinct-pointer-types */, 3167, /* DiagArray160 */ 480, 0, R"()")
DIAG_ENTRY(CompletionHandler /* completion-handler */, 3198, /* DiagArray161 */ 482, 0, R"()")
DIAG_ENTRY(anonymous_216 /* complex-component-init */, 3217, /* DiagArray162 */ 486, 0, R"()")
DIAG_ENTRY(CompoundTokenSplit /* compound-token-split */, 3240, 0, /* DiagSubGroup163 */ 156, R"()")
DIAG_ENTRY(CompoundTokenSplitByMacro /* compound-token-split-by-macro */, 3261, /* DiagArray164 */ 488, 0, R"()")
DIAG_ENTRY(CompoundTokenSplitBySpace /* compound-token-split-by-space */, 3291, /* DiagArray165 */ 490, 0, R"()")
DIAG_ENTRY(anonymous_249 /* conditional-type-mismatch */, 3321, /* DiagArray166 */ 492, 0, R"()")
DIAG_ENTRY(UninitializedMaybe /* conditional-uninitialized */, 3347, /* DiagArray167 */ 494, 0, R"()")
DIAG_ENTRY(ConfigMacros /* config-macros */, 3373, /* DiagArray168 */ 496, 0, R"()")
DIAG_ENTRY(ConstantConversion /* constant-conversion */, 3387, /* DiagArray169 */ 498, /* DiagSubGroup169 */ 159, R"()")
DIAG_ENTRY(anonymous_73 /* constant-evaluated */, 3407, /* DiagArray170 */ 500, 0, R"()")
DIAG_ENTRY(anonymous_226 /* constant-logical-operand */, 3426, /* DiagArray171 */ 502, 0, R"()")
DIAG_ENTRY(anonymous_171 /* constexpr-not-const */, 3451, /* DiagArray172 */ 504, 0, R"()")
DIAG_ENTRY(Consumed /* consumed */, 3471, /* DiagArray173 */ 506, 0, R"()")
DIAG_ENTRY(Conversion /* conversion */, 3480, /* DiagArray174 */ 515, /* DiagSubGroup174 */ 162, R"()")
DIAG_ENTRY(anonymous_58 /* conversion-null */, 3491, 0, /* DiagSubGroup175 */ 178, R"()")
DIAG_ENTRY(CoroNonAlignedAllocationFunction /* coro-non-aligned-allocation-funciton */, 3507, /* DiagArray176 */ 521, 0, R"()")
DIAG_ENTRY(Coroutine /* coroutine */, 3544, /* DiagArray177 */ 523, /* DiagSubGroup177 */ 180, R"()")
DIAG_ENTRY(CoroutineMissingUnhandledException /* coroutine-missing-unhandled-exception */, 3554, /* DiagArray178 */ 525, 0, R"()")
DIAG_ENTRY(CoveredSwitchDefault /* covered-switch-default */, 3592, /* DiagArray179 */ 527, 0, R"()")
DIAG_ENTRY(anonymous_56 /* cpp */, 3615, 0, /* DiagSubGroup180 */ 185, R"()")
DIAG_ENTRY(ObjCCStringFormat /* cstring-format-directive */, 3619, /* DiagArray181 */ 529, 0, R"()")
DIAG_ENTRY(CTADMaybeUnsupported /* ctad-maybe-unsupported */, 3644, /* DiagArray182 */ 531, 0, R"()")
DIAG_ENTRY(anonymous_4 /* ctor-dtor-privacy */, 3667, 0, 0, R"()")
DIAG_ENTRY(CrossTU /* ctu */, 3685, /* DiagArray184 */ 533, 0, R"()")
DIAG_ENTRY(CudaCompat /* cuda-compat */, 3689, /* DiagArray185 */ 535, 0, R"()")
DIAG_ENTRY(CustomAtomic /* custom-atomic-properties */, 3701, /* DiagArray186 */ 541, 0, R"()")
DIAG_ENTRY(anonymous_134 /* cxx-attribute-extension */, 3726, /* DiagArray187 */ 543, 0, R"()")
DIAG_ENTRY(Dangling /* dangling */, 3750, /* DiagArray188 */ 545, /* DiagSubGroup188 */ 187, R"()")
DIAG_ENTRY(DanglingElse /* dangling-else */, 3759, /* DiagArray189 */ 548, 0, R"()")
DIAG_ENTRY(DanglingField /* dangling-field */, 3773, /* DiagArray190 */ 550, 0, R"()")
DIAG_ENTRY(DanglingGsl /* dangling-gsl */, 3788, /* DiagArray191 */ 555, 0, R"()")
DIAG_ENTRY(DanglingInitializerList /* dangling-initializer-list */, 3801, /* DiagArray192 */ 558, 0, R"()")
DIAG_ENTRY(anonymous_86 /* darwin-sdk-settings */, 3827, /* DiagArray193 */ 560, 0, R"()")
DIAG_ENTRY(anonymous_119 /* date-time */, 3847, /* DiagArray194 */ 562, 0, R"()")
DIAG_ENTRY(DeallocInCategory /* dealloc-in-category */, 3857, /* DiagArray195 */ 564, 0, R"()")
DIAG_ENTRY(anonymous_83 /* debug-compression-unavailable */, 3877, /* DiagArray196 */ 566, 0, R"()")
DIAG_ENTRY(DeclarationAfterStatement /* declaration-after-statement */, 3907, /* DiagArray197 */ 568, 0, R"()")
DIAG_ENTRY(DefaultedFunctionDeleted /* defaulted-function-deleted */, 3935, /* DiagArray198 */ 571, 0, R"()")
DIAG_ENTRY(DelegatingCtorCycles /* delegating-ctor-cycles */, 3962, /* DiagArray199 */ 574, 0, R"()")
DIAG_ENTRY(DeleteAbstractNonVirtualDtor /* delete-abstract-non-virtual-dtor */, 3985, /* DiagArray200 */ 576, 0, R"()")
DIAG_ENTRY(DeleteIncomplete /* delete-incomplete */, 4018, /* DiagArray201 */ 578, 0, R"()")
DIAG_ENTRY(DeleteNonAbstractNonVirtualDtor /* delete-non-abstract-non-virtual-dtor */, 4036, /* DiagArray202 */ 581, 0, R"()")
DIAG_ENTRY(DeleteNonVirtualDtor /* delete-non-virtual-dtor */, 4073, 0, /* DiagSubGroup203 */ 192, R"()")
DIAG_ENTRY(anonymous_102 /* delimited-escape-sequence-extension */, 4097, /* DiagArray204 */ 583, 0, R"()")
DIAG_ENTRY(anonymous_239 /* deprecate-lax-vec-conv-all */, 4133, /* DiagArray205 */ 585, 0, R"()")
DIAG_ENTRY(Deprecated /* deprecated */, 4160, /* DiagArray206 */ 587, /* DiagSubGroup206 */ 195, R"()")
DIAG_ENTRY(anonymous_238 /* deprecated-altivec-src-compat */, 4171, /* DiagArray207 */ 595, 0, R"()")
DIAG_ENTRY(DeprecatedAnonEnumEnumConversion /* deprecated-anon-enum-enum-conversion */, 4201, /* DiagArray208 */ 597, 0, R"()")
DIAG_ENTRY(DeprecatedArrayCompare /* deprecated-array-compare */, 4238, /* DiagArray209 */ 599, 0, R"()")
DIAG_ENTRY(DeprecatedAttributes /* deprecated-attributes */, 4263, /* DiagArray210 */ 601, 0, R"()")
DIAG_ENTRY(DeprecatedBuiltins /* deprecated-builtins */, 4285, /* DiagArray211 */ 605, 0, R"()")
DIAG_ENTRY(DeprecatedCommaSubscript /* deprecated-comma-subscript */, 4305, /* DiagArray212 */ 607, 0, R"()")
DIAG_ENTRY(DeprecatedCopy /* deprecated-copy */, 4332, /* DiagArray213 */ 609, /* DiagSubGroup213 */ 216, R"()")
DIAG_ENTRY(anonymous_5 /* deprecated-copy-dtor */, 4348, 0, /* DiagSubGroup214 */ 218, R"()")
DIAG_ENTRY(DeprecatedCopyWithDtor /* deprecated-copy-with-dtor */, 4369, /* DiagArray215 */ 611, /* DiagSubGroup215 */ 220, R"()")
DIAG_ENTRY(DeprecatedCopyWithUserProvidedCopy /* deprecated-copy-with-user-provided-copy */, 4395, /* DiagArray216 */ 613, 0, R"()")
DIAG_ENTRY(DeprecatedCopyWithUserProvidedDtor /* deprecated-copy-with-user-provided-dtor */, 4435, /* DiagArray217 */ 615, 0, R"()")
DIAG_ENTRY(DeprecatedCoroutine /* deprecated-coroutine */, 4475, /* DiagArray218 */ 617, /* DiagSubGroup218 */ 222, R"()")
DIAG_ENTRY(DeprecatedDeclarations /* deprecated-declarations */, 4496, /* DiagArray219 */ 619, 0, R"()")
DIAG_ENTRY(DeprecatedDynamicExceptionSpec /* deprecated-dynamic-exception-spec */, 4520, /* DiagArray220 */ 626, 0, R"()")
DIAG_ENTRY(DeprecatedEnumCompare /* deprecated-enum-compare */, 4554, /* DiagArray221 */ 628, 0, R"()")
DIAG_ENTRY(DeprecatedEnumCompareConditional /* deprecated-enum-compare-conditional */, 4578, /* DiagArray222 */ 630, 0, R"()")
DIAG_ENTRY(DeprecatedEnumEnumConversion /* deprecated-enum-enum-conversion */, 4614, /* DiagArray223 */ 632, 0, R"()")
DIAG_ENTRY(DeprecatedEnumFloatConversion /* deprecated-enum-float-conversion */, 4646, /* DiagArray224 */ 634, 0, R"()")
DIAG_ENTRY(DeprecatedExperimentalCoroutine /* deprecated-experimental-coroutine */, 4679, /* DiagArray225 */ 636, 0, R"()")
DIAG_ENTRY(DeprecatedImplementations /* deprecated-implementations */, 4713, /* DiagArray226 */ 638, 0, R"()")
DIAG_ENTRY(DeprecatedIncrementBool /* deprecated-increment-bool */, 4740, /* DiagArray227 */ 641, 0, R"()")
DIAG_ENTRY(DeprecatedNonPrototype /* deprecated-non-prototype */, 4766, /* DiagArray228 */ 643, 0, R"()")
DIAG_ENTRY(DeprecatedObjCIsaUsage /* deprecated-objc-isa-usage */, 4791, /* DiagArray229 */ 646, 0, R"()")
DIAG_ENTRY(ObjCPointerIntrospect /* deprecated-objc-pointer-introspection */, 4817, /* DiagArray230 */ 649, /* DiagSubGroup230 */ 224, R"()")
DIAG_ENTRY(ObjCPointerIntrospectPerformSelector /* deprecated-objc-pointer-introspection-performSelector */, 4855, /* DiagArray231 */ 651, 0, R"()")
DIAG_ENTRY(DeprecatedPragma /* deprecated-pragma */, 4909, /* DiagArray232 */ 653, 0, R"()")
DIAG_ENTRY(DeprecatedRegister /* deprecated-register */, 4927, /* DiagArray233 */ 655, 0, R"()")
DIAG_ENTRY(DeprecatedStaticAnalyzerFlag /* deprecated-static-analyzer-flag */, 4947, /* DiagArray234 */ 657, 0, R"()")
DIAG_ENTRY(DeprecatedThisCapture /* deprecated-this-capture */, 4979, /* DiagArray235 */ 660, 0, R"()")
DIAG_ENTRY(DeprecatedType /* deprecated-type */, 5003, /* DiagArray236 */ 662, 0, R"()")
DIAG_ENTRY(DeprecatedVolatile /* deprecated-volatile */, 5019, /* DiagArray237 */ 664, 0, R"()")
DIAG_ENTRY(DeprecatedWritableStr /* deprecated-writable-strings */, 5039, 0, /* DiagSubGroup238 */ 226, R"()")
DIAG_ENTRY(anonymous_265 /* direct-ivar-access */, 5067, /* DiagArray239 */ 670, 0, R"()")
DIAG_ENTRY(anonymous_110 /* disabled-macro-expansion */, 5086, /* DiagArray240 */ 672, 0, R"()")
DIAG_ENTRY(anonymous_7 /* disabled-optimization */, 5111, 0, 0, R"()")
DIAG_ENTRY(anonymous_8 /* discard-qual */, 5133, 0, 0, R"()")
DIAG_ENTRY(DistributedObjectModifiers /* distributed-object-modifiers */, 5146, /* DiagArray243 */ 674, 0, R"()")
DIAG_ENTRY(anonymous_9 /* div-by-zero */, 5175, 0, /* DiagSubGroup244 */ 228, R"()")
DIAG_ENTRY(DivZero /* division-by-zero */, 5187, /* DiagArray245 */ 677, 0, R"()")
DIAG_ENTRY(anonymous_181 /* dll-attribute-on-redeclaration */, 5204, /* DiagArray246 */ 679, 0, R"()")
DIAG_ENTRY(DllexportExplicitInstantiationDecl /* dllexport-explicit-instantiation-decl */, 5235, /* DiagArray247 */ 681, 0, R"()")
DIAG_ENTRY(anonymous_182 /* dllimport-static-field-def */, 5273, /* DiagArray248 */ 683, 0, R"()")
DIAG_ENTRY(Documentation /* documentation */, 5300, /* DiagArray249 */ 685, /* DiagSubGroup249 */ 230, R"()")
DIAG_ENTRY(DocumentationDeprecatedSync /* documentation-deprecated-sync */, 5314, /* DiagArray250 */ 705, 0, R"()")
DIAG_ENTRY(DocumentationHTML /* documentation-html */, 5344, /* DiagArray251 */ 707, 0, R"()")
DIAG_ENTRY(DocumentationPedantic /* documentation-pedantic */, 5363, /* DiagArray252 */ 712, /* DiagSubGroup252 */ 233, R"()")
DIAG_ENTRY(DocumentationUnknownCommand /* documentation-unknown-command */, 5386, /* DiagArray253 */ 714, 0, R"()")
DIAG_ENTRY(anonymous_95 /* dollar-in-identifier-extension */, 5416, /* DiagArray254 */ 717, 0, R"()")
DIAG_ENTRY(DoublePromotion /* double-promotion */, 5447, /* DiagArray255 */ 719, 0, R"()")
DIAG_ENTRY(DtorName /* dtor-name */, 5464, /* DiagArray256 */ 721, 0, R"()")
DIAG_ENTRY(anonymous_170 /* dtor-typedef */, 5474, /* DiagArray257 */ 725, 0, R"()")
DIAG_ENTRY(DuplicateDeclSpecifier /* duplicate-decl-specifier */, 5487, /* DiagArray258 */ 727, 0, R"()")
DIAG_ENTRY(anonymous_137 /* duplicate-enum */, 5512, /* DiagArray259 */ 732, 0, R"()")
DIAG_ENTRY(DuplicateArgDecl /* duplicate-method-arg */, 5527, /* DiagArray260 */ 734, 0, R"()")
DIAG_ENTRY(MethodDuplicate /* duplicate-method-match */, 5548, /* DiagArray261 */ 736, 0, R"()")
DIAG_ENTRY(anonymous_151 /* duplicate-protocol */, 5571, /* DiagArray262 */ 738, 0, R"()")
DIAG_ENTRY(DynamicClassMemaccess /* dynamic-class-memaccess */, 5590, /* DiagArray263 */ 740, 0, R"()")
DIAG_ENTRY(DynamicExceptionSpec /* dynamic-exception-spec */, 5614, /* DiagArray264 */ 742, /* DiagSubGroup264 */ 235, R"()")
DIAG_ENTRY(anonymous_32 /* effc++ */, 5637, 0, /* DiagSubGroup265 */ 237, R"()")
DIAG_ENTRY(anonymous_125 /* elaborated-enum-base */, 5644, /* DiagArray266 */ 744, 0, R"()")
DIAG_ENTRY(anonymous_126 /* elaborated-enum-class */, 5665, /* DiagArray267 */ 746, 0, R"()")
DIAG_ENTRY(anonymous_116 /* embedded-directive */, 5687, /* DiagArray268 */ 748, 0, R"()")
DIAG_ENTRY(EmptyBody /* empty-body */, 5706, /* DiagArray269 */ 750, 0, R"()")
DIAG_ENTRY(anonymous_130 /* empty-decomposition */, 5717, /* DiagArray270 */ 756, 0, R"()")
DIAG_ENTRY(EmptyInitStatement /* empty-init-stmt */, 5737, /* DiagArray271 */ 758, 0, R"()")
DIAG_ENTRY(anonymous_122 /* empty-translation-unit */, 5753, /* DiagArray272 */ 760, 0, R"()")
DIAG_ENTRY(anonymous_185 /* encode-type */, 5776, /* DiagArray273 */ 762, 0, R"()")
DIAG_ENTRY(anonymous_55 /* endif-labels */, 5788, 0, /* DiagSubGroup274 */ 239, R"()")
DIAG_ENTRY(EnumCompare /* enum-compare */, 5801, /* DiagArray275 */ 764, /* DiagSubGroup275 */ 241, R"()")
DIAG_ENTRY(EnumCompareConditional /* enum-compare-conditional */, 5814, /* DiagArray276 */ 766, /* DiagSubGroup276 */ 244, R"()")
DIAG_ENTRY(EnumCompareSwitch /* enum-compare-switch */, 5839, /* DiagArray277 */ 768, 0, R"()")
DIAG_ENTRY(anonymous_72 /* enum-constexpr-conversion */, 5859, /* DiagArray278 */ 770, 0, R"()")
DIAG_ENTRY(EnumConversion /* enum-conversion */, 5885, /* DiagArray279 */ 772, /* DiagSubGroup279 */ 246, R"()")
DIAG_ENTRY(EnumEnumConversion /* enum-enum-conversion */, 5901, /* DiagArray280 */ 774, /* DiagSubGroup280 */ 250, R"()")
DIAG_ENTRY(EnumFloatConversion /* enum-float-conversion */, 5922, /* DiagArray281 */ 776, /* DiagSubGroup281 */ 252, R"()")
DIAG_ENTRY(EnumTooLarge /* enum-too-large */, 5944, /* DiagArray282 */ 778, 0, R"()")
DIAG_ENTRY(Exceptions /* exceptions */, 5959, /* DiagArray283 */ 781, 0, R"()")
DIAG_ENTRY(ExcessInitializers /* excess-initializers */, 5970, /* DiagArray284 */ 785, 0, R"()")
DIAG_ENTRY(ExitTimeDestructors /* exit-time-destructors */, 5990, /* DiagArray285 */ 790, 0, R"()")
DIAG_ENTRY(ExpansionToDefined /* expansion-to-defined */, 6012, /* DiagArray286 */ 792, 0, R"()")
DIAG_ENTRY(ExplicitInitializeCall /* explicit-initialize-call */, 6033, /* DiagArray287 */ 795, 0, R"()")
DIAG_ENTRY(anonymous_218 /* explicit-ownership-type */, 6058, /* DiagArray288 */ 798, 0, R"()")
DIAG_ENTRY(ExportUnnamed /* export-unnamed */, 6082, /* DiagArray289 */ 800, 0, R"()")
DIAG_ENTRY(anonymous_269 /* export-using-directive */, 6097, /* DiagArray290 */ 803, 0, R"()")
DIAG_ENTRY(ExternCCompat /* extern-c-compat */, 6120, /* DiagArray291 */ 805, 0, R"()")
DIAG_ENTRY(anonymous_213 /* extern-initializer */, 6136, /* DiagArray292 */ 807, 0, R"()")
DIAG_ENTRY(Extra /* extra */, 6155, /* DiagArray293 */ 809, /* DiagSubGroup293 */ 254, R"()")
DIAG_ENTRY(anonymous_227 /* extra-qualification */, 6161, /* DiagArray294 */ 811, 0, R"()")
DIAG_ENTRY(ExtraSemi /* extra-semi */, 6181, /* DiagArray295 */ 813, /* DiagSubGroup295 */ 269, R"()")
DIAG_ENTRY(ExtraSemiStmt /* extra-semi-stmt */, 6192, /* DiagArray296 */ 816, /* DiagSubGroup296 */ 272, R"()")
DIAG_ENTRY(ExtraTokens /* extra-tokens */, 6208, /* DiagArray297 */ 818, 0, R"()")
DIAG_ENTRY(FinalDtorNonFinalClass /* final-dtor-non-final-class */, 6221, /* DiagArray298 */ 821, 0, R"()")
DIAG_ENTRY(FinalMacro /* final-macro */, 6248, /* DiagArray299 */ 823, 0, R"()")
DIAG_ENTRY(anonymous_124 /* fixed-enum-extension */, 6260, /* DiagArray300 */ 825, 0, R"()")
DIAG_ENTRY(anonymous_71 /* fixed-point-overflow */, 6281, /* DiagArray301 */ 827, 0, R"()")
DIAG_ENTRY(FlagEnum /* flag-enum */, 6302, /* DiagArray302 */ 829, 0, R"()")
DIAG_ENTRY(FlexibleArrayExtensions /* flexible-array-extensions */, 6312, /* DiagArray303 */ 831, 0, R"()")
DIAG_ENTRY(FloatConversion /* float-conversion */, 6338, /* DiagArray304 */ 834, /* DiagSubGroup304 */ 274, R"()")
DIAG_ENTRY(anonymous_220 /* float-equal */, 6355, /* DiagArray305 */ 836, 0, R"()")
DIAG_ENTRY(FloatOverflowConversion /* float-overflow-conversion */, 6367, /* DiagArray306 */ 838, 0, R"()")
DIAG_ENTRY(FloatZeroConversion /* float-zero-conversion */, 6393, /* DiagArray307 */ 841, 0, R"()")
DIAG_ENTRY(ForLoopAnalysis /* for-loop-analysis */, 6415, /* DiagArray308 */ 843, 0, R"()")
DIAG_ENTRY(Format /* format */, 6433, /* DiagArray309 */ 846, /* DiagSubGroup309 */ 277, R"()")
DIAG_ENTRY(FormatExtraArgs /* format-extra-args */, 6440, /* DiagArray310 */ 873, 0, R"()")
DIAG_ENTRY(FormatInsufficientArgs /* format-insufficient-args */, 6458, /* DiagArray311 */ 875, 0, R"()")
DIAG_ENTRY(FormatInvalidSpecifier /* format-invalid-specifier */, 6483, /* DiagArray312 */ 877, 0, R"()")
DIAG_ENTRY(FormatNonStandard /* format-non-iso */, 6508, /* DiagArray313 */ 879, 0, R"()")
DIAG_ENTRY(FormatNonLiteral /* format-nonliteral */, 6523, /* DiagArray314 */ 883, 0, R"()")
DIAG_ENTRY(FormatPedantic /* format-pedantic */, 6541, /* DiagArray315 */ 885, 0, R"()")
DIAG_ENTRY(FormatSecurity /* format-security */, 6557, /* DiagArray316 */ 888, 0, R"()")
DIAG_ENTRY(FormatTypeConfusion /* format-type-confusion */, 6573, /* DiagArray317 */ 890, 0, R"()")
DIAG_ENTRY(FormatY2K /* format-y2k */, 6595, 0, 0, R"()")
DIAG_ENTRY(FormatZeroLength /* format-zero-length */, 6606, /* DiagArray319 */ 892, 0, R"()")
DIAG_ENTRY(Format2 /* format=2 */, 6625, 0, /* DiagSubGroup320 */ 285, R"()")
DIAG_ENTRY(FortifySource /* fortify-source */, 6634, /* DiagArray321 */ 894, 0, R"()")
DIAG_ENTRY(FourByteMultiChar /* four-char-constants */, 6649, /* DiagArray322 */ 900, 0, R"()")
DIAG_ENTRY(FrameAddress /* frame-address */, 6669, /* DiagArray323 */ 902, 0, R"()")
DIAG_ENTRY(BackendFrameLargerThan /* frame-larger-than */, 6683, /* DiagArray324 */ 904, 0, R"()")
DIAG_ENTRY(anonymous_68 /* frame-larger-than= */, 6701, 0, /* DiagSubGroup325 */ 289, R"()")
DIAG_ENTRY(FrameworkIncludePrivateFromPublic /* framework-include-private-from-public */, 6720, /* DiagArray326 */ 907, 0, R"()")
DIAG_ENTRY(FreeNonHeapObject /* free-nonheap-object */, 6758, /* DiagArray327 */ 909, 0, R"()")
DIAG_ENTRY(FunctionDefInObjCContainer /* function-def-in-objc-container */, 6778, /* DiagArray328 */ 911, 0, R"()")
DIAG_ENTRY(FunctionMultiVersioning /* function-multiversion */, 6809, /* DiagArray329 */ 913, /* DiagSubGroup329 */ 291, R"()")
DIAG_ENTRY(FUseLdPath /* fuse-ld-path */, 6831, /* DiagArray330 */ 918, 0, R"()")
DIAG_ENTRY(FutureAttrs /* future-attribute-extensions */, 6844, 0, /* DiagSubGroup331 */ 293, R"()")
DIAG_ENTRY(FutureCompat /* future-compat */, 6872, 0, 0, R"()")
DIAG_ENTRY(GccCompat /* gcc-compat */, 6886, /* DiagArray333 */ 920, 0, R"()")
DIAG_ENTRY(GlobalConstructors /* global-constructors */, 6897, /* DiagArray334 */ 932, 0, R"()")
DIAG_ENTRY(GlobalISel /* global-isel */, 6917, /* DiagArray335 */ 935, 0, R"()")
DIAG_ENTRY(GNU /* gnu */, 6929, 0, /* DiagSubGroup336 */ 297, R"()")
DIAG_ENTRY(GNUAlignofExpression /* gnu-alignof-expression */, 6933, /* DiagArray337 */ 938, 0, R"()")
DIAG_ENTRY(GNUAnonymousStruct /* gnu-anonymous-struct */, 6956, /* DiagArray338 */ 940, 0, R"()")
DIAG_ENTRY(anonymous_229 /* gnu-array-member-paren-init */, 6977, /* DiagArray339 */ 942, 0, R"()")
DIAG_ENTRY(GNUAutoType /* gnu-auto-type */, 7005, /* DiagArray340 */ 944, 0, R"()")
DIAG_ENTRY(GNUBinaryLiteral /* gnu-binary-literal */, 7019, /* DiagArray341 */ 946, 0, R"()")
DIAG_ENTRY(GNUCaseRange /* gnu-case-range */, 7038, /* DiagArray342 */ 948, 0, R"()")
DIAG_ENTRY(GNUComplexInteger /* gnu-complex-integer */, 7053, /* DiagArray343 */ 950, 0, R"()")
DIAG_ENTRY(GNUCompoundLiteralInitializer /* gnu-compound-literal-initializer */, 7073, /* DiagArray344 */ 952, 0, R"()")
DIAG_ENTRY(GNUConditionalOmittedOperand /* gnu-conditional-omitted-operand */, 7106, /* DiagArray345 */ 954, 0, R"()")
DIAG_ENTRY(GNUDesignator /* gnu-designator */, 7138, /* DiagArray346 */ 956, 0, R"()")
DIAG_ENTRY(GNUEmptyInitializer /* gnu-empty-initializer */, 7153, /* DiagArray347 */ 960, 0, R"()")
DIAG_ENTRY(GNUEmptyStruct /* gnu-empty-struct */, 7175, /* DiagArray348 */ 962, 0, R"()")
DIAG_ENTRY(GNUFlexibleArrayInitializer /* gnu-flexible-array-initializer */, 7192, /* DiagArray349 */ 966, 0, R"()")
DIAG_ENTRY(GNUFlexibleArrayUnionMember /* gnu-flexible-array-union-member */, 7223, /* DiagArray350 */ 968, 0, R"()")
DIAG_ENTRY(GNUFoldingConstant /* gnu-folding-constant */, 7255, /* DiagArray351 */ 970, 0, R"()")
DIAG_ENTRY(GNUImaginaryConstant /* gnu-imaginary-constant */, 7276, /* DiagArray352 */ 974, 0, R"()")
DIAG_ENTRY(GNUIncludeNext /* gnu-include-next */, 7299, /* DiagArray353 */ 976, 0, R"()")
DIAG_ENTRY(anonymous_186 /* gnu-inline-cpp-without-extern */, 7316, /* DiagArray354 */ 978, 0, R"()")
DIAG_ENTRY(GNULabelsAsValue /* gnu-label-as-value */, 7346, /* DiagArray355 */ 980, 0, R"()")
DIAG_ENTRY(GNULineMarker /* gnu-line-marker */, 7365, /* DiagArray356 */ 983, 0, R"()")
DIAG_ENTRY(GNUNullPointerArithmetic /* gnu-null-pointer-arithmetic */, 7381, /* DiagArray357 */ 985, 0, R"()")
DIAG_ENTRY(GNUPointerArith /* gnu-pointer-arith */, 7409, /* DiagArray358 */ 987, 0, R"()")
DIAG_ENTRY(GNURedeclaredEnum /* gnu-redeclared-enum */, 7427, /* DiagArray359 */ 991, 0, R"()")
DIAG_ENTRY(GNUStatementExpression /* gnu-statement-expression */, 7447, /* DiagArray360 */ 993, /* DiagSubGroup360 */ 329, R"()")
DIAG_ENTRY(GNUStatementExpressionFromMacroExpansion /* gnu-statement-expression-from-macro-expansion */, 7472, /* DiagArray361 */ 995, 0, R"()")
DIAG_ENTRY(GNUStaticFloatInit /* gnu-static-float-init */, 7518, /* DiagArray362 */ 997, 0, R"()")
DIAG_ENTRY(GNUStringLiteralOperatorTemplate /* gnu-string-literal-operator-template */, 7540, /* DiagArray363 */ 999, 0, R"()")
DIAG_ENTRY(GNUUnionCast /* gnu-union-cast */, 7577, /* DiagArray364 */ 1001, 0, R"()")
DIAG_ENTRY(GNUVariableSizedTypeNotAtEnd /* gnu-variable-sized-type-not-at-end */, 7592, /* DiagArray365 */ 1003, 0, R"()")
DIAG_ENTRY(GNUZeroLineDirective /* gnu-zero-line-directive */, 7627, /* DiagArray366 */ 1005, 0, R"()")
DIAG_ENTRY(GNUZeroVariadicMacroArguments /* gnu-zero-variadic-macro-arguments */, 7651, /* DiagArray367 */ 1007, 0, R"()")
DIAG_ENTRY(anonymous_246 /* gpu-maybe-wrong-side */, 7685, /* DiagArray368 */ 1010, 0, R"()")
DIAG_ENTRY(anonymous_121 /* header-guard */, 7706, /* DiagArray369 */ 1012, 0, R"()")
DIAG_ENTRY(HeaderHygiene /* header-hygiene */, 7719, /* DiagArray370 */ 1014, 0, R"()")
DIAG_ENTRY(HIPOnly /* hip-only */, 7734, /* DiagArray371 */ 1016, 0, R"()")
DIAG_ENTRY(HLSLExtension /* hlsl-extensions */, 7743, /* DiagArray372 */ 1018, 0, R"()")
DIAG_ENTRY(anonymous_241 /* idiomatic-parentheses */, 7759, /* DiagArray373 */ 1020, 0, R"()")
DIAG_ENTRY(IgnoredAttributes /* ignored-attributes */, 7781, /* DiagArray374 */ 1022, 0, R"()")
DIAG_ENTRY(anonymous_189 /* ignored-availability-without-sdk-settings */, 7800, /* DiagArray375 */ 1106, 0, R"()")
DIAG_ENTRY(IgnoredOptimizationArgument /* ignored-optimization-argument */, 7842, /* DiagArray376 */ 1108, 0, R"()")
DIAG_ENTRY(IgnoredPragmaIntrinsic /* ignored-pragma-intrinsic */, 7872, /* DiagArray377 */ 1111, 0, R"()")
DIAG_ENTRY(IgnoredPragmaOptimize /* ignored-pragma-optimize */, 7897, 0, 0, R"()")
DIAG_ENTRY(IgnoredPragmas /* ignored-pragmas */, 7921, /* DiagArray379 */ 1113, /* DiagSubGroup379 */ 331, R"()")
DIAG_ENTRY(IgnoredQualifiers /* ignored-qualifiers */, 7937, /* DiagArray380 */ 1162, /* DiagSubGroup380 */ 334, R"()")
DIAG_ENTRY(IgnoredReferenceQualifiers /* ignored-reference-qualifiers */, 7956, /* DiagArray381 */ 1167, 0, R"()")
DIAG_ENTRY(Implicit /* implicit */, 7985, 0, /* DiagSubGroup382 */ 336, R"()")
DIAG_ENTRY(ImplicitAtomic /* implicit-atomic-properties */, 7994, /* DiagArray383 */ 1169, 0, R"()")
DIAG_ENTRY(ImplicitConstIntFloatConversion /* implicit-const-int-float-conversion */, 8021, /* DiagArray384 */ 1172, 0, R"()")
DIAG_ENTRY(ImplicitConversionFloatingPointToBool /* implicit-conversion-floating-point-to-bool */, 8057, /* DiagArray385 */ 1174, 0, R"()")
DIAG_ENTRY(anonymous_257 /* implicit-exception-spec-mismatch */, 8100, /* DiagArray386 */ 1176, 0, R"()")
DIAG_ENTRY(ImplicitFallthrough /* implicit-fallthrough */, 8133, /* DiagArray387 */ 1178, /* DiagSubGroup387 */ 339, R"()")
DIAG_ENTRY(ImplicitFallthroughPerFunction /* implicit-fallthrough-per-function */, 8154, /* DiagArray388 */ 1180, 0, R"()")
DIAG_ENTRY(ImplicitFixedPointConversion /* implicit-fixed-point-conversion */, 8188, /* DiagArray389 */ 1182, 0, R"()")
DIAG_ENTRY(ImplicitFloatConversion /* implicit-float-conversion */, 8220, /* DiagArray390 */ 1184, /* DiagSubGroup390 */ 341, R"()")
DIAG_ENTRY(ImplicitFunctionDeclare /* implicit-function-declaration */, 8246, /* DiagArray391 */ 1187, 0, R"()")
DIAG_ENTRY(ImplicitInt /* implicit-int */, 8276, /* DiagArray392 */ 1193, 0, R"()")
DIAG_ENTRY(ImplicitIntConversion /* implicit-int-conversion */, 8289, /* DiagArray393 */ 1197, /* DiagSubGroup393 */ 344, R"()")
DIAG_ENTRY(ImplicitIntFloatConversion /* implicit-int-float-conversion */, 8313, /* DiagArray394 */ 1200, /* DiagSubGroup394 */ 346, R"()")
DIAG_ENTRY(anonymous_162 /* implicit-retain-self */, 8343, /* DiagArray395 */ 1202, 0, R"()")
DIAG_ENTRY(ImplicitlyUnsignedLiteral /* implicitly-unsigned-literal */, 8364, /* DiagArray396 */ 1204, 0, R"()")
DIAG_ENTRY(anonymous_23 /* import */, 8392, 0, 0, R"()")
DIAG_ENTRY(anonymous_115 /* import-preprocessor-directive-pedantic */, 8399, /* DiagArray398 */ 1206, 0, R"()")
DIAG_ENTRY(anonymous_254 /* inaccessible-base */, 8438, /* DiagArray399 */ 1208, 0, R"()")
DIAG_ENTRY(anonymous_105 /* include-next-absolute-path */, 8456, /* DiagArray400 */ 1210, 0, R"()")
DIAG_ENTRY(anonymous_104 /* include-next-outside-header */, 8483, /* DiagArray401 */ 1212, 0, R"()")
DIAG_ENTRY(IncompatibleExceptionSpec /* incompatible-exception-spec */, 8511, /* DiagArray402 */ 1214, 0, R"()")
DIAG_ENTRY(IncompatibleFunctionPointerTypes /* incompatible-function-pointer-types */, 8539, /* DiagArray403 */ 1217, 0, R"()")
DIAG_ENTRY(anonymous_243 /* incompatible-function-pointer-types-strict */, 8575, /* DiagArray404 */ 1219, 0, R"()")
DIAG_ENTRY(anonymous_145 /* incompatible-library-redeclaration */, 8618, /* DiagArray405 */ 1221, 0, R"()")
DIAG_ENTRY(IncompatibleMSStruct /* incompatible-ms-struct */, 8653, /* DiagArray406 */ 1223, 0, R"()")
DIAG_ENTRY(IncompatiblePointerTypes /* incompatible-pointer-types */, 8676, /* DiagArray407 */ 1226, /* DiagSubGroup407 */ 348, R"()")
DIAG_ENTRY(IncompatiblePointerTypesDiscardsQualifiers /* incompatible-pointer-types-discards-qualifiers */, 8703, /* DiagArray408 */ 1228, 0, R"()")
DIAG_ENTRY(anonymous_152 /* incompatible-property-type */, 8750, /* DiagArray409 */ 1232, 0, R"()")
DIAG_ENTRY(anonymous_82 /* incompatible-sysroot */, 8777, /* DiagArray410 */ 1234, 0, R"()")
DIAG_ENTRY(IncompleteFrameworkModuleDeclaration /* incomplete-framework-module-declaration */, 8798, /* DiagArray411 */ 1236, 0, R"()")
DIAG_ENTRY(anonymous_154 /* incomplete-implementation */, 8838, /* DiagArray412 */ 1238, 0, R"()")
DIAG_ENTRY(IncompleteModule /* incomplete-module */, 8864, 0, /* DiagSubGroup413 */ 351, R"()")
DIAG_ENTRY(anonymous_144 /* incomplete-setjmp-declaration */, 8882, /* DiagArray414 */ 1240, 0, R"()")
DIAG_ENTRY(IncompleteUmbrella /* incomplete-umbrella */, 8912, /* DiagArray415 */ 1242, 0, R"()")
DIAG_ENTRY(MicrosoftInconsistentDllImport /* inconsistent-dllimport */, 8932, /* DiagArray416 */ 1246, 0, R"()")
DIAG_ENTRY(CXX11WarnInconsistentOverrideDestructor /* inconsistent-missing-destructor-override */, 8955, /* DiagArray417 */ 1249, 0, R"()")
DIAG_ENTRY(CXX11WarnInconsistentOverrideMethod /* inconsistent-missing-override */, 8996, /* DiagArray418 */ 1251, 0, R"()")
DIAG_ENTRY(IncrementBool /* increment-bool */, 9026, /* DiagArray419 */ 1253, /* DiagSubGroup419 */ 354, R"()")
DIAG_ENTRY(InfiniteRecursion /* infinite-recursion */, 9041, /* DiagArray420 */ 1255, 0, R"()")
DIAG_ENTRY(anonymous_24 /* init-self */, 9060, 0, 0, R"()")
DIAG_ENTRY(InitializerOverrides /* initializer-overrides */, 9070, /* DiagArray422 */ 1257, 0, R"()")
DIAG_ENTRY(anonymous_169 /* injected-class-name */, 9092, /* DiagArray423 */ 1260, 0, R"()")
DIAG_ENTRY(anonymous_25 /* inline */, 9112, 0, 0, R"()")
DIAG_ENTRY(BackendInlineAsm /* inline-asm */, 9119, /* DiagArray425 */ 1262, 0, R"()")
DIAG_ENTRY(InlineNamespaceReopenedNoninline /* inline-namespace-reopened-noninline */, 9130, /* DiagArray426 */ 1264, 0, R"()")
DIAG_ENTRY(anonymous_255 /* inline-new-delete */, 9166, /* DiagArray427 */ 1266, 0, R"()")
DIAG_ENTRY(anonymous_202 /* instantiation-after-specialization */, 9184, /* DiagArray428 */ 1268, 0, R"()")
DIAG_ENTRY(IntConversion /* int-conversion */, 9219, /* DiagArray429 */ 1270, 0, R"()")
DIAG_ENTRY(anonymous_60 /* int-conversions */, 9234, 0, /* DiagSubGroup430 */ 356, R"()")
DIAG_ENTRY(IntInBoolContext /* int-in-bool-context */, 9250, /* DiagArray431 */ 1273, 0, R"()")
DIAG_ENTRY(IntToPointerCast /* int-to-pointer-cast */, 9270, /* DiagArray432 */ 1276, /* DiagSubGroup432 */ 358, R"()")
DIAG_ENTRY(IntToVoidPointerCast /* int-to-void-pointer-cast */, 9290, /* DiagArray433 */ 1278, 0, R"()")
DIAG_ENTRY(anonymous_70 /* integer-overflow */, 9315, /* DiagArray434 */ 1280, 0, R"()")
DIAG_ENTRY(anonymous_140 /* interrupt-service-routine */, 9332, /* DiagArray435 */ 1282, 0, R"()")
DIAG_ENTRY(InvalidCommandLineArgument /* invalid-command-line-argument */, 9358, /* DiagArray436 */ 1284, /* DiagSubGroup436 */ 360, R"()")
DIAG_ENTRY(anonymous_172 /* invalid-constexpr */, 9388, /* DiagArray437 */ 1294, 0, R"()")
DIAG_ENTRY(ObjCInvalidIBOutletProperty /* invalid-iboutlet */, 9406, /* DiagArray438 */ 1296, 0, R"()")
DIAG_ENTRY(anonymous_183 /* invalid-initializer-from-system-header */, 9423, /* DiagArray439 */ 1299, 0, R"()")
DIAG_ENTRY(InvalidIOSDeploymentTarget /* invalid-ios-deployment-target */, 9462, /* DiagArray440 */ 1301, 0, R"()")
DIAG_ENTRY(anonymous_195 /* invalid-no-builtin-names */, 9492, /* DiagArray441 */ 1303, 0, R"()")
DIAG_ENTRY(InvalidNoreturn /* invalid-noreturn */, 9517, /* DiagArray442 */ 1305, 0, R"()")
DIAG_ENTRY(InvalidOffsetof /* invalid-offsetof */, 9534, /* DiagArray443 */ 1308, 0, R"()")
DIAG_ENTRY(InvalidOrNonExistentDirectory /* invalid-or-nonexistent-directory */, 9551, /* DiagArray444 */ 1311, 0, R"()")
DIAG_ENTRY(anonymous_200 /* invalid-partial-specialization */, 9584, /* DiagArray445 */ 1314, 0, R"()")
DIAG_ENTRY(anonymous_26 /* invalid-pch */, 9615, 0, 0, R"()")
DIAG_ENTRY(InvalidPPToken /* invalid-pp-token */, 9627, /* DiagArray447 */ 1316, 0, R"()")
DIAG_ENTRY(InvalidSourceEncoding /* invalid-source-encoding */, 9644, /* DiagArray448 */ 1319, 0, R"()")
DIAG_ENTRY(anonymous_118 /* invalid-token-paste */, 9668, /* DiagArray449 */ 1322, 0, R"()")
DIAG_ENTRY(anonymous_97 /* invalid-utf8 */, 9688, /* DiagArray450 */ 1324, 0, R"()")
DIAG_ENTRY(anonymous_240 /* jump-seh-finally */, 9701, /* DiagArray451 */ 1326, 0, R"()")
DIAG_ENTRY(KeywordCompat /* keyword-compat */, 9718, /* DiagArray452 */ 1328, 0, R"()")
DIAG_ENTRY(KeywordAsMacro /* keyword-macro */, 9733, /* DiagArray453 */ 1330, 0, R"()")
DIAG_ENTRY(KNRPromotedParameter /* knr-promoted-parameter */, 9747, /* DiagArray454 */ 1332, 0, R"()")
DIAG_ENTRY(anonymous_96 /* language-extension-token */, 9770, /* DiagArray455 */ 1334, 0, R"()")
DIAG_ENTRY(LargeByValueCopy /* large-by-value-copy */, 9795, /* DiagArray456 */ 1336, 0, R"()")
DIAG_ENTRY(LibLTO /* liblto */, 9815, 0, 0, R"()")
DIAG_ENTRY(LinkerWarnings /* linker-warnings */, 9822, /* DiagArray458 */ 1339, 0, R"()")
DIAG_ENTRY(LiteralConversion /* literal-conversion */, 9838, /* DiagArray459 */ 1341, 0, R"()")
DIAG_ENTRY(LiteralRange /* literal-range */, 9857, /* DiagArray460 */ 1344, 0, R"()")
DIAG_ENTRY(LocalTypeTemplateArgs /* local-type-template-args */, 9871, /* DiagArray461 */ 1348, /* DiagSubGroup461 */ 362, R"()")
DIAG_ENTRY(LogicalNotParentheses /* logical-not-parentheses */, 9896, /* DiagArray462 */ 1350, 0, R"()")
DIAG_ENTRY(LogicalOpParentheses /* logical-op-parentheses */, 9920, /* DiagArray463 */ 1352, 0, R"()")
DIAG_ENTRY(LongLong /* long-long */, 9943, /* DiagArray464 */ 1354, /* DiagSubGroup464 */ 364, R"()")
DIAG_ENTRY(LoopAnalysis /* loop-analysis */, 9953, 0, /* DiagSubGroup465 */ 366, R"()")
DIAG_ENTRY(MacroRedefined /* macro-redefined */, 9967, /* DiagArray466 */ 1356, 0, R"()")
DIAG_ENTRY(Main /* main */, 9983, /* DiagArray467 */ 1358, 0, R"()")
DIAG_ENTRY(MainReturnType /* main-return-type */, 9988, /* DiagArray468 */ 1366, 0, R"()")
DIAG_ENTRY(MalformedWarningCheck /* malformed-warning-check */, 10005, /* DiagArray469 */ 1368, 0, R"()")
DIAG_ENTRY(anonymous_215 /* many-braces-around-scalar-init */, 10029, /* DiagArray470 */ 1370, 0, R"()")
DIAG_ENTRY(anonymous_101 /* mathematical-notation-identifier-extension */, 10060, /* DiagArray471 */ 1372, 0, R"()")
DIAG_ENTRY(MaxTokens /* max-tokens */, 10103, /* DiagArray472 */ 1374, 0, R"(The warning is issued if the number of pre-processor tokens exceeds
the token limit, which can be set in three ways:

1. As a limit at a specific point in a file, using the ``clang max_tokens_here``
   pragma:

   .. code-block: c++
      #pragma clang max_tokens_here 1234

2. As a per-translation unit limit, using the ``-fmax-tokens=`` command-line
   flag:

   .. code-block: console
      clang -c a.cpp -fmax-tokens=1234

3. As a per-translation unit limit using the ``clang max_tokens_total`` pragma,
   which works like and overrides the ``-fmax-tokens=`` flag:

   .. code-block: c++
      #pragma clang max_tokens_total 1234

These limits can be helpful in limiting code growth through included files.

Setting a token limit of zero means no limit.

Note that the warning is disabled by default, so -Wmax-tokens must be used
in addition with the pragmas or -fmax-tokens flag to get any warnings.)")
DIAG_ENTRY(MaxUnsignedZero /* max-unsigned-zero */, 10114, /* DiagArray473 */ 1377, 0, R"()")
DIAG_ENTRY(MemsetTransposedArgs /* memset-transposed-args */, 10132, /* DiagArray474 */ 1379, 0, R"()")
DIAG_ENTRY(anonymous_147 /* memsize-comparison */, 10155, /* DiagArray475 */ 1381, 0, R"()")
DIAG_ENTRY(MethodSignatures /* method-signatures */, 10174, /* DiagArray476 */ 1383, 0, R"()")
DIAG_ENTRY(Microsoft /* microsoft */, 10192, 0, /* DiagSubGroup477 */ 369, R"()")
DIAG_ENTRY(MicrosoftAbstract /* microsoft-abstract */, 10202, /* DiagArray478 */ 1386, 0, R"()")
DIAG_ENTRY(MicrosoftAnonTag /* microsoft-anon-tag */, 10221, /* DiagArray479 */ 1388, 0, R"()")
DIAG_ENTRY(MicrosoftCast /* microsoft-cast */, 10240, /* DiagArray480 */ 1391, 0, R"()")
DIAG_ENTRY(MicrosoftCharize /* microsoft-charize */, 10255, /* DiagArray481 */ 1394, 0, R"()")
DIAG_ENTRY(MicrosoftCommentPaste /* microsoft-comment-paste */, 10273, /* DiagArray482 */ 1396, 0, R"()")
DIAG_ENTRY(MicrosoftConstInit /* microsoft-const-init */, 10297, /* DiagArray483 */ 1398, 0, R"()")
DIAG_ENTRY(MicrosoftCppMacro /* microsoft-cpp-macro */, 10318, /* DiagArray484 */ 1400, 0, R"()")
DIAG_ENTRY(MicrosoftDefaultArgRedefinition /* microsoft-default-arg-redefinition */, 10338, /* DiagArray485 */ 1402, 0, R"()")
DIAG_ENTRY(MicrosoftDrectveSection /* microsoft-drectve-section */, 10373, /* DiagArray486 */ 1404, 0, R"()")
DIAG_ENTRY(MicrosoftEndOfFile /* microsoft-end-of-file */, 10399, /* DiagArray487 */ 1406, 0, R"()")
DIAG_ENTRY(MicrosoftEnumForwardReference /* microsoft-enum-forward-reference */, 10421, /* DiagArray488 */ 1408, 0, R"()")
DIAG_ENTRY(MicrosoftEnumValue /* microsoft-enum-value */, 10454, /* DiagArray489 */ 1410, 0, R"()")
DIAG_ENTRY(MicrosoftExceptionSpec /* microsoft-exception-spec */, 10475, /* DiagArray490 */ 1412, 0, R"()")
DIAG_ENTRY(anonymous_127 /* microsoft-exists */, 10500, /* DiagArray491 */ 1418, 0, R"()")
DIAG_ENTRY(MicrosoftExplicitConstructorCall /* microsoft-explicit-constructor-call */, 10517, /* DiagArray492 */ 1420, 0, R"()")
DIAG_ENTRY(MicrosoftExtraQualification /* microsoft-extra-qualification */, 10553, /* DiagArray493 */ 1422, 0, R"()")
DIAG_ENTRY(MicrosoftFixedEnum /* microsoft-fixed-enum */, 10583, /* DiagArray494 */ 1424, 0, R"()")
DIAG_ENTRY(MicrosoftFlexibleArray /* microsoft-flexible-array */, 10604, /* DiagArray495 */ 1426, 0, R"()")
DIAG_ENTRY(MicrosoftGoto /* microsoft-goto */, 10629, /* DiagArray496 */ 1429, 0, R"()")
DIAG_ENTRY(MicrosoftInaccessibleBase /* microsoft-inaccessible-base */, 10644, /* DiagArray497 */ 1431, 0, R"()")
DIAG_ENTRY(MicrosoftInclude /* microsoft-include */, 10672, /* DiagArray498 */ 1433, 0, R"()")
DIAG_ENTRY(MicrosoftMutableReference /* microsoft-mutable-reference */, 10690, /* DiagArray499 */ 1435, 0, R"()")
DIAG_ENTRY(MicrosoftPureDefinition /* microsoft-pure-definition */, 10718, /* DiagArray500 */ 1437, 0, R"()")
DIAG_ENTRY(MicrosoftRedeclareStatic /* microsoft-redeclare-static */, 10744, /* DiagArray501 */ 1439, 0, R"()")
DIAG_ENTRY(MicrosoftSealed /* microsoft-sealed */, 10771, /* DiagArray502 */ 1441, 0, R"()")
DIAG_ENTRY(MicrosoftStaticAssert /* microsoft-static-assert */, 10788, /* DiagArray503 */ 1443, 0, R"()")
DIAG_ENTRY(MicrosoftTemplate /* microsoft-template */, 10812, /* DiagArray504 */ 1445, /* DiagSubGroup504 */ 400, R"()")
DIAG_ENTRY(MicrosoftTemplateShadow /* microsoft-template-shadow */, 10831, /* DiagArray505 */ 1456, 0, R"()")
DIAG_ENTRY(MicrosoftUnionMemberReference /* microsoft-union-member-reference */, 10857, /* DiagArray506 */ 1458, 0, R"()")
DIAG_ENTRY(MicrosoftUnqualifiedFriend /* microsoft-unqualified-friend */, 10890, /* DiagArray507 */ 1460, 0, R"()")
DIAG_ENTRY(MicrosoftUsingDecl /* microsoft-using-decl */, 10919, /* DiagArray508 */ 1462, 0, R"()")
DIAG_ENTRY(MicrosoftVoidPseudoDtor /* microsoft-void-pseudo-dtor */, 10940, /* DiagArray509 */ 1464, 0, R"()")
DIAG_ENTRY(MisExpect /* misexpect */, 10967, /* DiagArray510 */ 1466, 0, R"()")
DIAG_ENTRY(MisleadingIndentation /* misleading-indentation */, 10977, /* DiagArray511 */ 1468, 0, R"()")
DIAG_ENTRY(anonymous_236 /* mismatched-new-delete */, 11000, /* DiagArray512 */ 1470, 0, R"()")
DIAG_ENTRY(MismatchedParameterTypes /* mismatched-parameter-types */, 11022, /* DiagArray513 */ 1472, 0, R"()")
DIAG_ENTRY(MismatchedReturnTypes /* mismatched-return-types */, 11049, /* DiagArray514 */ 1474, 0, R"()")
DIAG_ENTRY(MismatchedTags /* mismatched-tags */, 11073, /* DiagArray515 */ 1476, 0, R"()")
DIAG_ENTRY(MissingBraces /* missing-braces */, 11089, /* DiagArray516 */ 1479, 0, R"()")
DIAG_ENTRY(anonymous_251 /* missing-constinit */, 11104, /* DiagArray517 */ 1481, 0, R"()")
DIAG_ENTRY(MissingDeclarations /* missing-declarations */, 11122, /* DiagArray518 */ 1483, 0, R"()")
DIAG_ENTRY(anonymous_168 /* missing-exception-spec */, 11143, /* DiagArray519 */ 1488, 0, R"()")
DIAG_ENTRY(MissingFieldInitializers /* missing-field-initializers */, 11166, /* DiagArray520 */ 1490, 0, R"()")
DIAG_ENTRY(anonymous_27 /* missing-format-attribute */, 11193, 0, 0, R"()")
DIAG_ENTRY(anonymous_28 /* missing-include-dirs */, 11218, 0, 0, R"()")
DIAG_ENTRY(MissingMethodReturnType /* missing-method-return-type */, 11239, /* DiagArray523 */ 1492, 0, R"()")
DIAG_ENTRY(MissingNoEscape /* missing-noescape */, 11266, /* DiagArray524 */ 1494, 0, R"()")
DIAG_ENTRY(MissingNoreturn /* missing-noreturn */, 11283, /* DiagArray525 */ 1496, 0, R"()")
DIAG_ENTRY(anonymous_187 /* missing-prototype-for-cc */, 11300, /* DiagArray526 */ 1499, 0, R"()")
DIAG_ENTRY(anonymous_204 /* missing-prototypes */, 11325, /* DiagArray527 */ 1501, 0, R"()")
DIAG_ENTRY(anonymous_128 /* missing-selector-name */, 11344, /* DiagArray528 */ 1503, 0, R"()")
DIAG_ENTRY(anonymous_81 /* missing-sysroot */, 11366, /* DiagArray529 */ 1505, 0, R"()")
DIAG_ENTRY(anonymous_205 /* missing-variable-declarations */, 11382, /* DiagArray530 */ 1507, 0, R"()")
DIAG_ENTRY(MisspelledAssumption /* misspelled-assumption */, 11412, /* DiagArray531 */ 1509, 0, R"()")
DIAG_ENTRY(ModuleBuild /* module-build */, 11434, /* DiagArray532 */ 1511, 0, R"()")
DIAG_ENTRY(ModuleConflict /* module-conflict */, 11447, /* DiagArray533 */ 1516, 0, R"()")
DIAG_ENTRY(anonymous_92 /* module-file-config-mismatch */, 11463, /* DiagArray534 */ 1519, 0, R"()")
DIAG_ENTRY(ModuleFileExtension /* module-file-extension */, 11491, /* DiagArray535 */ 1521, 0, R"()")
DIAG_ENTRY(ModuleImport /* module-import */, 11513, /* DiagArray536 */ 1523, 0, R"()")
DIAG_ENTRY(anonymous_267 /* module-import-in-extern-c */, 11527, /* DiagArray537 */ 1525, 0, R"()")
DIAG_ENTRY(ModuleIncludeDirectiveTranslation /* module-include-translation */, 11553, /* DiagArray538 */ 1527, 0, R"()")
DIAG_ENTRY(ModuleLock /* module-lock */, 11580, /* DiagArray539 */ 1529, 0, R"()")
DIAG_ENTRY(anonymous_270 /* modules-ambiguous-internal-linkage */, 11592, /* DiagArray540 */ 1531, 0, R"()")
DIAG_ENTRY(anonymous_268 /* modules-import-nested-redundant */, 11627, /* DiagArray541 */ 1533, 0, R"()")
DIAG_ENTRY(Most /* most */, 11659, 0, /* DiagSubGroup542 */ 402, R"()")
DIAG_ENTRY(Move /* move */, 11664, 0, /* DiagSubGroup543 */ 440, R"()")
DIAG_ENTRY(anonymous_67 /* msvc-include */, 11669, 0, /* DiagSubGroup544 */ 445, R"()")
DIAG_ENTRY(anonymous_85 /* msvc-not-found */, 11682, /* DiagArray545 */ 1535, 0, R"()")
DIAG_ENTRY(MultiChar /* multichar */, 11697, /* DiagArray546 */ 1537, 0, R"()")
DIAG_ENTRY(anonymous_256 /* multiple-move-vbase */, 11707, /* DiagArray547 */ 1539, 0, R"()")
DIAG_ENTRY(anonymous_18 /* narrowing */, 11727, 0, /* DiagSubGroup548 */ 447, R"()")
DIAG_ENTRY(anonymous_253 /* nested-anon-types */, 11737, /* DiagArray549 */ 1541, 0, R"()")
DIAG_ENTRY(anonymous_29 /* nested-externs */, 11755, 0, 0, R"()")
DIAG_ENTRY(OperatorNewReturnsNull /* new-returns-null */, 11770, /* DiagArray551 */ 1543, 0, R"()")
DIAG_ENTRY(NewlineEOF /* newline-eof */, 11787, /* DiagArray552 */ 1545, 0, R"()")
DIAG_ENTRY(NoDeref /* noderef */, 11799, /* DiagArray553 */ 1548, 0, R"()")
DIAG_ENTRY(NoexceptType /* noexcept-type */, 11807, 0, /* DiagSubGroup554 */ 449, R"()")
DIAG_ENTRY(anonymous_150 /* non-c-typedef-for-linkage */, 11821, /* DiagArray555 */ 1552, 0, R"()")
DIAG_ENTRY(NonGCC /* non-gcc */, 11847, 0, /* DiagSubGroup556 */ 451, R"()")
DIAG_ENTRY(NonLiteralNullConversion /* non-literal-null-conversion */, 11855, /* DiagArray557 */ 1554, 0, R"()")
DIAG_ENTRY(NonModularIncludeInFrameworkModule /* non-modular-include-in-framework-module */, 11883, /* DiagArray558 */ 1556, 0, R"()")
DIAG_ENTRY(NonModularIncludeInModule /* non-modular-include-in-module */, 11923, /* DiagArray559 */ 1558, /* DiagSubGroup559 */ 455, R"()")
DIAG_ENTRY(NonPODVarargs /* non-pod-varargs */, 11953, /* DiagArray560 */ 1560, 0, R"()")
DIAG_ENTRY(anonymous_178 /* non-power-of-two-alignment */, 11969, /* DiagArray561 */ 1565, 0, R"()")
DIAG_ENTRY(NonVirtualDtor /* non-virtual-dtor */, 11996, /* DiagArray562 */ 1567, 0, R"()")
DIAG_ENTRY(NonNull /* nonnull */, 12013, /* DiagArray563 */ 1569, 0, R"()")
DIAG_ENTRY(anonymous_31 /* nonportable-cfstrings */, 12021, 0, 0, R"()")
DIAG_ENTRY(anonymous_106 /* nonportable-include-path */, 12043, /* DiagArray565 */ 1572, 0, R"()")
DIAG_ENTRY(anonymous_107 /* nonportable-system-include-path */, 12068, /* DiagArray566 */ 1574, 0, R"()")
DIAG_ENTRY(anonymous_262 /* nonportable-vector-initialization */, 12100, /* DiagArray567 */ 1576, 0, R"()")
DIAG_ENTRY(NonTrivialMemaccess /* nontrivial-memaccess */, 12134, /* DiagArray568 */ 1578, 0, R"()")
DIAG_ENTRY(NSConsumedMismatch /* nsconsumed-mismatch */, 12155, /* DiagArray569 */ 1580, 0, R"()")
DIAG_ENTRY(NSReturnsMismatch /* nsreturns-mismatch */, 12175, /* DiagArray570 */ 1582, 0, R"()")
DIAG_ENTRY(NullArithmetic /* null-arithmetic */, 12194, /* DiagArray571 */ 1584, 0, R"()")
DIAG_ENTRY(NullCharacter /* null-character */, 12210, /* DiagArray572 */ 1587, 0, R"()")
DIAG_ENTRY(NullConversion /* null-conversion */, 12225, /* DiagArray573 */ 1590, 0, R"()")
DIAG_ENTRY(NullDereference /* null-dereference */, 12241, /* DiagArray574 */ 1592, 0, R"()")
DIAG_ENTRY(NullPointerArithmetic /* null-pointer-arithmetic */, 12258, /* DiagArray575 */ 1595, /* DiagSubGroup575 */ 457, R"()")
DIAG_ENTRY(NullPointerSubtraction /* null-pointer-subtraction */, 12282, /* DiagArray576 */ 1597, 0, R"()")
DIAG_ENTRY(Nullability /* nullability */, 12307, /* DiagArray577 */ 1599, 0, R"()")
DIAG_ENTRY(NullabilityCompleteness /* nullability-completeness */, 12319, /* DiagArray578 */ 1605, /* DiagSubGroup578 */ 459, R"()")
DIAG_ENTRY(NullabilityCompletenessOnArrays /* nullability-completeness-on-arrays */, 12344, /* DiagArray579 */ 1607, 0, R"()")
DIAG_ENTRY(NullabilityDeclSpec /* nullability-declspec */, 12379, /* DiagArray580 */ 1609, 0, R"()")
DIAG_ENTRY(anonymous_123 /* nullability-extension */, 12400, /* DiagArray581 */ 1611, 0, R"()")
DIAG_ENTRY(NullabilityInferredOnNestedType /* nullability-inferred-on-nested-type */, 12422, /* DiagArray582 */ 1613, 0, R"()")
DIAG_ENTRY(NullableToNonNullConversion /* nullable-to-nonnull-conversion */, 12458, /* DiagArray583 */ 1615, 0, R"()")
DIAG_ENTRY(anonymous_159 /* objc-autosynthesis-property-ivar-name-match */, 12489, /* DiagArray584 */ 1617, 0, R"()")
DIAG_ENTRY(ObjCBoolConstantConversion /* objc-bool-constant-conversion */, 12533, /* DiagArray585 */ 1619, 0, R"()")
DIAG_ENTRY(ObjCBoxing /* objc-boxing */, 12563, /* DiagArray586 */ 1621, 0, R"()")
DIAG_ENTRY(anonymous_233 /* objc-circular-container */, 12575, /* DiagArray587 */ 1623, 0, R"()")
DIAG_ENTRY(ObjCCocoaAPI /* objc-cocoa-api */, 12599, 0, /* DiagSubGroup588 */ 461, R"()")
DIAG_ENTRY(ObjCDesignatedInit /* objc-designated-initializers */, 12614, /* DiagArray589 */ 1625, 0, R"()")
DIAG_ENTRY(anonymous_175 /* objc-dictionary-duplicate-keys */, 12643, /* DiagArray590 */ 1632, 0, R"()")
DIAG_ENTRY(ObjCFlexibleArray /* objc-flexible-array */, 12674, /* DiagArray591 */ 1634, 0, R"()")
DIAG_ENTRY(anonymous_211 /* objc-forward-class-redefinition */, 12694, /* DiagArray592 */ 1637, 0, R"()")
DIAG_ENTRY(anonymous_212 /* objc-interface-ivars */, 12726, /* DiagArray593 */ 1639, 0, R"()")
DIAG_ENTRY(ObjCLiteralComparison /* objc-literal-compare */, 12747, /* DiagArray594 */ 1641, /* DiagSubGroup594 */ 463, R"()")
DIAG_ENTRY(ObjCLiteralConversion /* objc-literal-conversion */, 12768, /* DiagArray595 */ 1643, 0, R"()")
DIAG_ENTRY(anonymous_114 /* objc-macro-redefinition */, 12792, /* DiagArray596 */ 1646, 0, R"()")
DIAG_ENTRY(anonymous_164 /* objc-messaging-id */, 12816, /* DiagArray597 */ 1648, 0, R"()")
DIAG_ENTRY(MethodAccess /* objc-method-access */, 12834, /* DiagArray598 */ 1650, 0, R"()")
DIAG_ENTRY(anonymous_160 /* objc-missing-property-synthesis */, 12853, /* DiagArray599 */ 1657, 0, R"()")
DIAG_ENTRY(ObjCMissingSuperCalls /* objc-missing-super-calls */, 12885, /* DiagArray600 */ 1659, 0, R"()")
DIAG_ENTRY(ObjCMultipleMethodNames /* objc-multiple-method-names */, 12910, /* DiagArray601 */ 1661, 0, R"()")
DIAG_ENTRY(ObjCRetainBlockProperty /* objc-noncopy-retain-block-property */, 12937, /* DiagArray602 */ 1663, 0, R"()")
DIAG_ENTRY(ObjCNonUnifiedException /* objc-nonunified-exceptions */, 12972, /* DiagArray603 */ 1665, 0, R"()")
DIAG_ENTRY(ObjCPropertyAssignOnObjectType /* objc-property-assign-on-object-type */, 12999, /* DiagArray604 */ 1667, 0, R"()")
DIAG_ENTRY(ObjCPropertyImpl /* objc-property-implementation */, 13035, /* DiagArray605 */ 1669, 0, R"()")
DIAG_ENTRY(anonymous_155 /* objc-property-implicit-mismatch */, 13064, /* DiagArray606 */ 1674, 0, R"()")
DIAG_ENTRY(anonymous_157 /* objc-property-matches-cocoa-ownership-rule */, 13096, /* DiagArray607 */ 1676, 0, R"()")
DIAG_ENTRY(ObjCPropertyNoAttribute /* objc-property-no-attribute */, 13139, /* DiagArray608 */ 1678, 0, R"()")
DIAG_ENTRY(ObjCNoPropertyAutoSynthesis /* objc-property-synthesis */, 13166, /* DiagArray609 */ 1681, 0, R"()")
DIAG_ENTRY(ObjCProtocolMethodImpl /* objc-protocol-method-implementation */, 13190, /* DiagArray610 */ 1685, 0, R"()")
DIAG_ENTRY(anonymous_158 /* objc-protocol-property-synthesis */, 13226, /* DiagArray611 */ 1687, 0, R"()")
DIAG_ENTRY(ObjCProtocolQualifiers /* objc-protocol-qualifiers */, 13259, /* DiagArray612 */ 1689, 0, R"()")
DIAG_ENTRY(ObjCReadonlyPropertyHasSetter /* objc-readonly-with-setter-property */, 13284, /* DiagArray613 */ 1691, 0, R"()")
DIAG_ENTRY(ObjCRedundantAPIUse /* objc-redundant-api-use */, 13319, 0, /* DiagSubGroup614 */ 465, R"()")
DIAG_ENTRY(ObjCRedundantLiteralUse /* objc-redundant-literal-use */, 13342, /* DiagArray615 */ 1693, 0, R"()")
DIAG_ENTRY(ObjCRootClass /* objc-root-class */, 13369, /* DiagArray616 */ 1695, 0, R"()")
DIAG_ENTRY(ObjCSignedCharBool /* objc-signed-char-bool */, 13385, 0, /* DiagSubGroup617 */ 467, R"()")
DIAG_ENTRY(ObjCSignedCharBoolImplicitFloatConversion /* objc-signed-char-bool-implicit-float-conversion */, 13407, /* DiagArray618 */ 1697, 0, R"()")
DIAG_ENTRY(ObjCSignedCharBoolImplicitIntConversion /* objc-signed-char-bool-implicit-int-conversion */, 13455, /* DiagArray619 */ 1699, 0, R"()")
DIAG_ENTRY(ObjCStringComparison /* objc-string-compare */, 13501, /* DiagArray620 */ 1701, 0, R"()")
DIAG_ENTRY(ObjCStringConcatenation /* objc-string-concatenation */, 13521, /* DiagArray621 */ 1703, 0, R"()")
DIAG_ENTRY(anonymous_234 /* objc-unsafe-perform-selector */, 13547, /* DiagArray622 */ 1705, 0, R"()")
DIAG_ENTRY(ODR /* odr */, 13576, /* DiagArray623 */ 1707, 0, R"()")
DIAG_ENTRY(OldStyleCast /* old-style-cast */, 13580, /* DiagArray624 */ 1726, 0, R"()")
DIAG_ENTRY(anonymous_33 /* old-style-definition */, 13595, 0, 0, R"()")
DIAG_ENTRY(OpenCLUnsupportedRGBA /* opencl-unsupported-rgba */, 13616, /* DiagArray626 */ 1728, 0, R"()")
DIAG_ENTRY(OpenMP /* openmp */, 13640, 0, /* DiagSubGroup627 */ 472, R"()")
DIAG_ENTRY(OpenMP51Ext /* openmp-51-extensions */, 13647, /* DiagArray628 */ 1730, 0, R"()")
DIAG_ENTRY(OpenMPClauses /* openmp-clauses */, 13668, /* DiagArray629 */ 1732, 0, R"()")
DIAG_ENTRY(OpenMPLoopForm /* openmp-loop-form */, 13683, /* DiagArray630 */ 1752, 0, R"()")
DIAG_ENTRY(OpenMPMapping /* openmp-mapping */, 13700, /* DiagArray631 */ 1755, 0, R"()")
DIAG_ENTRY(OpenMPTarget /* openmp-target */, 13715, /* DiagArray632 */ 1757, /* DiagSubGroup632 */ 479, R"()")
DIAG_ENTRY(OptionIgnored /* option-ignored */, 13729, /* DiagArray633 */ 1761, 0, R"()")
DIAG_ENTRY(OrderedCompareFunctionPointers /* ordered-compare-function-pointers */, 13744, /* DiagArray634 */ 1776, 0, R"()")
DIAG_ENTRY(OutOfLineDeclaration /* out-of-line-declaration */, 13778, /* DiagArray635 */ 1779, 0, R"()")
DIAG_ENTRY(anonymous_142 /* out-of-scope-function */, 13802, /* DiagArray636 */ 1781, 0, R"()")
DIAG_ENTRY(OveralignedType /* over-aligned */, 13824, /* DiagArray637 */ 1783, 0, R"()")
DIAG_ENTRY(anonymous_34 /* overflow */, 13837, 0, 0, R"()")
DIAG_ENTRY(OverlengthStrings /* overlength-strings */, 13846, /* DiagArray639 */ 1785, 0, R"()")
DIAG_ENTRY(OverloadedShiftOpParentheses /* overloaded-shift-op-parentheses */, 13865, /* DiagArray640 */ 1787, 0, R"()")
DIAG_ENTRY(OverloadedVirtual /* overloaded-virtual */, 13897, /* DiagArray641 */ 1789, 0, R"()")
DIAG_ENTRY(anonymous_30 /* override-init */, 13916, 0, /* DiagSubGroup642 */ 481, R"()")
DIAG_ENTRY(anonymous_89 /* override-module */, 13930, /* DiagArray643 */ 1791, 0, R"()")
DIAG_ENTRY(OverridingMethodMismatch /* overriding-method-mismatch */, 13946, /* DiagArray644 */ 1793, 0, R"()")
DIAG_ENTRY(anonymous_80 /* overriding-t-option */, 13973, /* DiagArray645 */ 1801, 0, R"()")
DIAG_ENTRY(Packed /* packed */, 13993, /* DiagArray646 */ 1803, /* DiagSubGroup646 */ 483, R"()")
DIAG_ENTRY(PackedNonPod /* packed-non-pod */, 14000, /* DiagArray647 */ 1805, 0, R"()")
DIAG_ENTRY(Padded /* padded */, 14015, /* DiagArray648 */ 1807, 0, R"()")
DIAG_ENTRY(Parentheses /* parentheses */, 14022, /* DiagArray649 */ 1811, /* DiagSubGroup649 */ 485, R"()")
DIAG_ENTRY(ParenthesesOnEquality /* parentheses-equality */, 14034, /* DiagArray650 */ 1815, 0, R"()")
DIAG_ENTRY(anonymous_6 /* partial-availability */, 14055, 0, /* DiagSubGroup651 */ 494, R"()")
DIAG_ENTRY(BackendOptimizationRemark /* pass */, 14076, /* DiagArray652 */ 1817, 0, R"()")
DIAG_ENTRY(BackendOptimizationRemarkAnalysis /* pass-analysis */, 14081, /* DiagArray653 */ 1819, 0, R"()")
DIAG_ENTRY(BackendOptimizationFailure /* pass-failed */, 14095, /* DiagArray654 */ 1823, 0, R"()")
DIAG_ENTRY(BackendOptimizationRemarkMissed /* pass-missed */, 14107, /* DiagArray655 */ 1825, 0, R"()")
DIAG_ENTRY(anonymous_273 /* pch-date-time */, 14119, /* DiagArray656 */ 1827, 0, R"()")
DIAG_ENTRY(Pedantic /* pedantic */, 14133, /* DiagArray657 */ 1829, /* DiagSubGroup657 */ 496, R"()")
DIAG_ENTRY(OpenCLCoreFeaturesDiagGroup /* pedantic-core-features */, 14142, /* DiagArray658 */ 1904, 0, R"()")
DIAG_ENTRY(PedanticMacros /* pedantic-macros */, 14165, 0, /* DiagSubGroup659 */ 556, R"()")
DIAG_ENTRY(PessimizingMove /* pessimizing-move */, 14181, /* DiagArray660 */ 1907, 0, R"()")
DIAG_ENTRY(PointerArith /* pointer-arith */, 14198, /* DiagArray661 */ 1910, /* DiagSubGroup661 */ 562, R"()")
DIAG_ENTRY(PointerBoolConversion /* pointer-bool-conversion */, 14212, /* DiagArray662 */ 1914, 0, R"()")
DIAG_ENTRY(anonymous_190 /* pointer-compare */, 14236, /* DiagArray663 */ 1917, 0, R"()")
DIAG_ENTRY(anonymous_232 /* pointer-integer-compare */, 14252, /* DiagArray664 */ 1919, 0, R"()")
DIAG_ENTRY(anonymous_242 /* pointer-sign */, 14276, /* DiagArray665 */ 1921, 0, R"()")
DIAG_ENTRY(PointerToEnumCast /* pointer-to-enum-cast */, 14289, /* DiagArray666 */ 1923, /* DiagSubGroup666 */ 564, R"()")
DIAG_ENTRY(PointerToIntCast /* pointer-to-int-cast */, 14310, /* DiagArray667 */ 1925, /* DiagSubGroup667 */ 566, R"()")
DIAG_ENTRY(anonymous_248 /* pointer-type-mismatch */, 14330, /* DiagArray668 */ 1927, 0, R"()")
DIAG_ENTRY(anonymous_78 /* poison-system-directories */, 14352, /* DiagArray669 */ 1929, 0, R"()")
DIAG_ENTRY(ObjCPotentiallyDirectSelector /* potentially-direct-selector */, 14378, /* DiagArray670 */ 1931, 0, R"()")
DIAG_ENTRY(PotentiallyEvaluatedExpression /* potentially-evaluated-expression */, 14406, /* DiagArray671 */ 1933, 0, R"()")
DIAG_ENTRY(PragmaClangAttribute /* pragma-clang-attribute */, 14439, /* DiagArray672 */ 1935, 0, R"()")
DIAG_ENTRY(anonymous_108 /* pragma-once-outside-header */, 14462, /* DiagArray673 */ 1937, 0, R"()")
DIAG_ENTRY(PragmaPack /* pragma-pack */, 14489, /* DiagArray674 */ 1939, /* DiagSubGroup674 */ 569, R"()")
DIAG_ENTRY(PragmaPackSuspiciousInclude /* pragma-pack-suspicious-include */, 14501, /* DiagArray675 */ 1942, 0, R"()")
DIAG_ENTRY(anonymous_109 /* pragma-system-header-outside-header */, 14532, /* DiagArray676 */ 1944, 0, R"()")
DIAG_ENTRY(Pragmas /* pragmas */, 14568, /* DiagArray677 */ 1946, /* DiagSubGroup677 */ 571, R"()")
DIAG_ENTRY(CXXPre14Compat /* pre-c++14-compat */, 14576, /* DiagArray678 */ 1949, 0, R"()")
DIAG_ENTRY(CXXPre14CompatPedantic /* pre-c++14-compat-pedantic */, 14593, 0, /* DiagSubGroup679 */ 576, R"()")
DIAG_ENTRY(CXXPre17Compat /* pre-c++17-compat */, 14619, /* DiagArray680 */ 1961, 0, R"()")
DIAG_ENTRY(CXXPre17CompatPedantic /* pre-c++17-compat-pedantic */, 14636, /* DiagArray681 */ 1979, /* DiagSubGroup681 */ 579, R"()")
DIAG_ENTRY(CXXPre20Compat /* pre-c++20-compat */, 14662, /* DiagArray682 */ 1982, 0, R"()")
DIAG_ENTRY(CXXPre20CompatPedantic /* pre-c++20-compat-pedantic */, 14679, /* DiagArray683 */ 2010, /* DiagSubGroup683 */ 581, R"()")
DIAG_ENTRY(CXXPre2bCompat /* pre-c++2b-compat */, 14705, /* DiagArray684 */ 2013, 0, R"()")
DIAG_ENTRY(CXXPre2bCompatPedantic /* pre-c++2b-compat-pedantic */, 14722, 0, /* DiagSubGroup685 */ 583, R"()")
DIAG_ENTRY(CPre2xCompat /* pre-c2x-compat */, 14748, /* DiagArray686 */ 2028, 0, R"()")
DIAG_ENTRY(CPre2xCompatPedantic /* pre-c2x-compat-pedantic */, 14763, 0, /* DiagSubGroup687 */ 585, R"()")
DIAG_ENTRY(OpenMPPre51Compat /* pre-openmp-51-compat */, 14787, /* DiagArray688 */ 2039, 0, R"()")
DIAG_ENTRY(anonymous_139 /* predefined-identifier-outside-function */, 14808, /* DiagArray689 */ 2041, 0, R"()")
DIAG_ENTRY(PrivateExtern /* private-extern */, 14847, /* DiagArray690 */ 2043, 0, R"()")
DIAG_ENTRY(anonymous_120 /* private-header */, 14862, /* DiagArray691 */ 2045, 0, R"()")
DIAG_ENTRY(PrivateModule /* private-module */, 14877, /* DiagArray692 */ 2047, 0, R"()")
DIAG_ENTRY(ProfileInstrMissing /* profile-instr-missing */, 14892, /* DiagArray693 */ 2052, 0, R"()")
DIAG_ENTRY(ProfileInstrOutOfDate /* profile-instr-out-of-date */, 14914, /* DiagArray694 */ 2054, 0, R"()")
DIAG_ENTRY(ProfileInstrUnprofiled /* profile-instr-unprofiled */, 14940, /* DiagArray695 */ 2056, 0, R"()")
DIAG_ENTRY(PropertyAccessDotSyntax /* property-access-dot-syntax */, 14965, /* DiagArray696 */ 2058, 0, R"()")
DIAG_ENTRY(PropertyAttr /* property-attribute-mismatch */, 14992, /* DiagArray697 */ 2060, 0, R"()")
DIAG_ENTRY(Protocol /* protocol */, 15020, /* DiagArray698 */ 2065, 0, R"()")
DIAG_ENTRY(anonymous_153 /* protocol-property-synthesis-ambiguity */, 15029, /* DiagArray699 */ 2067, 0, R"()")
DIAG_ENTRY(anonymous_93 /* psabi */, 15067, /* DiagArray700 */ 2069, 0, R"()")
DIAG_ENTRY(anonymous_217 /* qualified-void-return-type */, 15073, /* DiagArray701 */ 2071, 0, R"()")
DIAG_ENTRY(FrameworkHdrQuotedInclude /* quoted-include-in-framework-header */, 15100, /* DiagArray702 */ 2073, 0, R"()")
DIAG_ENTRY(RangeLoopAnalysis /* range-loop-analysis */, 15135, 0, /* DiagSubGroup703 */ 587, R"()")
DIAG_ENTRY(RangeLoopBindReference /* range-loop-bind-reference */, 15155, /* DiagArray704 */ 2075, 0, R"()")
DIAG_ENTRY(RangeLoopConstruct /* range-loop-construct */, 15181, /* DiagArray705 */ 2077, 0, R"()")
DIAG_ENTRY(ReadOnlyPlacementChecks /* read-only-types */, 15202, /* DiagArray706 */ 2080, 0, R"()")
DIAG_ENTRY(anonymous_163 /* readonly-iboutlet-property */, 15218, /* DiagArray707 */ 2082, 0, R"()")
DIAG_ENTRY(ObjCReceiver /* receiver-expr */, 15245, /* DiagArray708 */ 2084, 0, R"()")
DIAG_ENTRY(ForwardClassReceiver /* receiver-forward-class */, 15259, /* DiagArray709 */ 2086, 0, R"()")
DIAG_ENTRY(RedeclaredClassMember /* redeclared-class-member */, 15282, /* DiagArray710 */ 2089, 0, R"()")
DIAG_ENTRY(anonymous_165 /* redundant-consteval-if */, 15306, /* DiagArray711 */ 2091, 0, R"()")
DIAG_ENTRY(anonymous_35 /* redundant-decls */, 15329, 0, 0, R"()")
DIAG_ENTRY(RedundantMove /* redundant-move */, 15345, /* DiagArray713 */ 2093, 0, R"()")
DIAG_ENTRY(anonymous_141 /* redundant-parens */, 15360, /* DiagArray714 */ 2095, 0, R"()")
DIAG_ENTRY(Register /* register */, 15377, /* DiagArray715 */ 2097, /* DiagSubGroup715 */ 590, R"()")
DIAG_ENTRY(ReinterpretBaseClass /* reinterpret-base-class */, 15386, /* DiagArray716 */ 2099, 0, R"()")
DIAG_ENTRY(RemarkBackendPlugin /* remark-backend-plugin */, 15409, /* DiagArray717 */ 2101, 0, R"()")
DIAG_ENTRY(Reorder /* reorder */, 15431, 0, /* DiagSubGroup718 */ 592, R"()")
DIAG_ENTRY(ReorderCtor /* reorder-ctor */, 15439, /* DiagArray719 */ 2103, 0, R"()")
DIAG_ENTRY(ReorderInitList /* reorder-init-list */, 15452, /* DiagArray720 */ 2106, 0, R"()")
DIAG_ENTRY(anonymous_196 /* requires-super-attribute */, 15470, /* DiagArray721 */ 2108, 0, R"()")
DIAG_ENTRY(ReservedIdAsMacroAlias /* reserved-id-macro */, 15495, 0, /* DiagSubGroup722 */ 595, R"()")
DIAG_ENTRY(ReservedIdentifier /* reserved-identifier */, 15513, /* DiagArray723 */ 2110, /* DiagSubGroup723 */ 597, R"()")
DIAG_ENTRY(ReservedIdAsMacro /* reserved-macro-identifier */, 15533, /* DiagArray724 */ 2112, 0, R"()")
DIAG_ENTRY(ReservedUserDefinedLiteral /* reserved-user-defined-literal */, 15559, /* DiagArray725 */ 2114, /* DiagSubGroup725 */ 599, R"()")
DIAG_ENTRY(RestrictExpansionMacro /* restrict-expansion */, 15589, /* DiagArray726 */ 2117, 0, R"()")
DIAG_ENTRY(anonymous_210 /* retained-language-linkage */, 15608, /* DiagArray727 */ 2119, 0, R"()")
DIAG_ENTRY(ReturnStackAddress /* return-stack-address */, 15634, /* DiagArray728 */ 2121, 0, R"()")
DIAG_ENTRY(ReturnStdMove /* return-std-move */, 15655, 0, 0, R"()")
DIAG_ENTRY(ReturnType /* return-type */, 15671, /* DiagArray730 */ 2125, /* DiagSubGroup730 */ 601, R"()")
DIAG_ENTRY(ReturnTypeCLinkage /* return-type-c-linkage */, 15683, /* DiagArray731 */ 2135, 0, R"()")
DIAG_ENTRY(anonymous_198 /* rewrite-not-bool */, 15705, /* DiagArray732 */ 2138, 0, R"()")
DIAG_ENTRY(RoundTripCC1Args /* round-trip-cc1-args */, 15722, /* DiagArray733 */ 2140, 0, R"()")
DIAG_ENTRY(RTTI /* rtti */, 15742, /* DiagArray734 */ 2142, 0, R"()")
DIAG_ENTRY(SanitizeAddressRemarks /* sanitize-address */, 15747, /* DiagArray735 */ 2145, 0, R"()")
DIAG_ENTRY(anonymous_88 /* sarif-format-unstable */, 15764, /* DiagArray736 */ 2148, 0, R"()")
DIAG_ENTRY(UsedSearchPath /* search-path-usage */, 15786, /* DiagArray737 */ 2150, 0, R"()")
DIAG_ENTRY(Section /* section */, 15804, /* DiagArray738 */ 2152, 0, R"()")
DIAG_ENTRY(Selector /* selector */, 15812, /* DiagArray739 */ 2156, /* DiagSubGroup739 */ 603, R"()")
DIAG_ENTRY(SelectorTypeMismatch /* selector-type-mismatch */, 15821, /* DiagArray740 */ 2158, 0, R"()")
DIAG_ENTRY(SelfAssignment /* self-assign */, 15844, /* DiagArray741 */ 2160, /* DiagSubGroup741 */ 605, R"()")
DIAG_ENTRY(SelfAssignmentField /* self-assign-field */, 15856, /* DiagArray742 */ 2162, 0, R"()")
DIAG_ENTRY(SelfAssignmentOverloaded /* self-assign-overloaded */, 15874, /* DiagArray743 */ 2164, 0, R"()")
DIAG_ENTRY(SelfMove /* self-move */, 15897, /* DiagArray744 */ 2166, 0, R"()")
DIAG_ENTRY(SemiBeforeMethodBody /* semicolon-before-method-body */, 15907, /* DiagArray745 */ 2168, 0, R"()")
DIAG_ENTRY(Sentinel /* sentinel */, 15936, /* DiagArray746 */ 2170, 0, R"()")
DIAG_ENTRY(anonymous_39 /* sequence-point */, 15945, 0, /* DiagSubGroup747 */ 608, R"()")
DIAG_ENTRY(SerializedDiagnostics /* serialized-diagnostics */, 15960, /* DiagArray748 */ 2173, 0, R"()")
DIAG_ENTRY(Shadow /* shadow */, 15983, /* DiagArray749 */ 2177, /* DiagSubGroup749 */ 610, R"()")
DIAG_ENTRY(ShadowAll /* shadow-all */, 15990, 0, /* DiagSubGroup750 */ 613, R"()")
DIAG_ENTRY(ShadowField /* shadow-field */, 16001, /* DiagArray751 */ 2179, 0, R"()")
DIAG_ENTRY(ShadowFieldInConstructor /* shadow-field-in-constructor */, 16014, /* DiagArray752 */ 2181, /* DiagSubGroup752 */ 618, R"()")
DIAG_ENTRY(ShadowFieldInConstructorModified /* shadow-field-in-constructor-modified */, 16042, /* DiagArray753 */ 2183, 0, R"()")
DIAG_ENTRY(ShadowIvar /* shadow-ivar */, 16079, /* DiagArray754 */ 2185, 0, R"()")
DIAG_ENTRY(ShadowUncapturedLocal /* shadow-uncaptured-local */, 16091, /* DiagArray755 */ 2187, 0, R"()")
DIAG_ENTRY(anonymous_222 /* shift-count-negative */, 16115, /* DiagArray756 */ 2189, 0, R"()")
DIAG_ENTRY(anonymous_223 /* shift-count-overflow */, 16136, /* DiagArray757 */ 2191, 0, R"()")
DIAG_ENTRY(anonymous_221 /* shift-negative-value */, 16157, /* DiagArray758 */ 2193, 0, R"()")
DIAG_ENTRY(ShiftOpParentheses /* shift-op-parentheses */, 16178, /* DiagArray759 */ 2195, 0, R"()")
DIAG_ENTRY(anonymous_224 /* shift-overflow */, 16199, /* DiagArray760 */ 2197, 0, R"()")
DIAG_ENTRY(anonymous_225 /* shift-sign-overflow */, 16214, /* DiagArray761 */ 2199, 0, R"()")
DIAG_ENTRY(Shorten64To32 /* shorten-64-to-32 */, 16234, /* DiagArray762 */ 2201, 0, R"()")
DIAG_ENTRY(SignCompare /* sign-compare */, 16251, /* DiagArray763 */ 2203, 0, R"()")
DIAG_ENTRY(SignConversion /* sign-conversion */, 16264, /* DiagArray764 */ 2205, 0, R"()")
DIAG_ENTRY(anonymous_36 /* sign-promo */, 16280, 0, 0, R"()")
DIAG_ENTRY(SignedEnumBitfield /* signed-enum-bitfield */, 16291, /* DiagArray766 */ 2209, 0, R"()")
DIAG_ENTRY(anonymous_264 /* signed-unsigned-wchar */, 16312, /* DiagArray767 */ 2211, 0, R"()")
DIAG_ENTRY(SingleBitBitFieldConstantConversion /* single-bit-bitfield-constant-conversion */, 16334, /* DiagArray768 */ 2213, 0, R"()")
DIAG_ENTRY(SizeofArrayArgument /* sizeof-array-argument */, 16374, /* DiagArray769 */ 2215, 0, R"()")
DIAG_ENTRY(SizeofArrayDecay /* sizeof-array-decay */, 16396, /* DiagArray770 */ 2217, 0, R"()")
DIAG_ENTRY(anonymous_192 /* sizeof-array-div */, 16415, /* DiagArray771 */ 2219, 0, R"()")
DIAG_ENTRY(anonymous_191 /* sizeof-pointer-div */, 16432, /* DiagArray772 */ 2221, 0, R"()")
DIAG_ENTRY(SizeofPointerMemaccess /* sizeof-pointer-memaccess */, 16451, /* DiagArray773 */ 2223, 0, R"()")
DIAG_ENTRY(anonymous_84 /* slash-u-filename */, 16476, /* DiagArray774 */ 2226, 0, R"()")
DIAG_ENTRY(anonymous_76 /* slh-asm-goto */, 16493, /* DiagArray775 */ 2228, 0, R"()")
DIAG_ENTRY(anonymous_77 /* sloc-usage */, 16506, /* DiagArray776 */ 2230, 0, R"()")
DIAG_ENTRY(UninitializedSometimes /* sometimes-uninitialized */, 16517, /* DiagArray777 */ 2232, 0, R"()")
DIAG_ENTRY(BackendSourceMgr /* source-mgr */, 16541, /* DiagArray778 */ 2234, 0, R"()")
DIAG_ENTRY(SourceUsesOpenMP /* source-uses-openmp */, 16552, /* DiagArray779 */ 2236, 0, R"()")
DIAG_ENTRY(SpirCompat /* spir-compat */, 16571, /* DiagArray780 */ 2245, 0, R"()")
DIAG_ENTRY(anonymous_69 /* spirv-compat */, 16583, 0, /* DiagSubGroup781 */ 620, R"()")
DIAG_ENTRY(anonymous_74 /* stack-exhausted */, 16596, /* DiagArray782 */ 2247, 0, R"()")
DIAG_ENTRY(anonymous_75 /* stack-protector */, 16612, /* DiagArray783 */ 2249, 0, R"()")
DIAG_ENTRY(StaticFloatInit /* static-float-init */, 16628, /* DiagArray784 */ 2251, /* DiagSubGroup784 */ 622, R"()")
DIAG_ENTRY(StaticInInline /* static-in-inline */, 16646, /* DiagArray785 */ 2253, 0, R"()")
DIAG_ENTRY(anonymous_135 /* static-inline-explicit-instantiation */, 16663, /* DiagArray786 */ 2256, 0, R"()")
DIAG_ENTRY(StaticLocalInInline /* static-local-in-inline */, 16700, /* DiagArray787 */ 2258, 0, R"()")
DIAG_ENTRY(UninitializedStaticSelfInit /* static-self-init */, 16723, /* DiagArray788 */ 2260, 0, R"()")
DIAG_ENTRY(anonymous_87 /* stdlibcxx-not-found */, 16740, /* DiagArray789 */ 2262, 0, R"()")
DIAG_ENTRY(anonymous_43 /* strict-aliasing */, 16760, 0, 0, R"()")
DIAG_ENTRY(anonymous_40 /* strict-aliasing=0 */, 16776, 0, 0, R"()")
DIAG_ENTRY(anonymous_41 /* strict-aliasing=1 */, 16794, 0, 0, R"()")
DIAG_ENTRY(anonymous_42 /* strict-aliasing=2 */, 16812, 0, 0, R"()")
DIAG_ENTRY(anonymous_50 /* strict-overflow */, 16830, 0, 0, R"()")
DIAG_ENTRY(anonymous_44 /* strict-overflow=0 */, 16846, 0, 0, R"()")
DIAG_ENTRY(anonymous_45 /* strict-overflow=1 */, 16864, 0, 0, R"()")
DIAG_ENTRY(anonymous_46 /* strict-overflow=2 */, 16882, 0, 0, R"()")
DIAG_ENTRY(anonymous_47 /* strict-overflow=3 */, 16900, 0, 0, R"()")
DIAG_ENTRY(anonymous_48 /* strict-overflow=4 */, 16918, 0, 0, R"()")
DIAG_ENTRY(anonymous_49 /* strict-overflow=5 */, 16936, 0, 0, R"()")
DIAG_ENTRY(ObjCStrictPotentiallyDirectSelector /* strict-potentially-direct-selector */, 16954, /* DiagArray801 */ 2264, /* DiagSubGroup801 */ 624, R"()")
DIAG_ENTRY(StrictPrototypes /* strict-prototypes */, 16989, /* DiagArray802 */ 2266, /* DiagSubGroup802 */ 626, R"()")
DIAG_ENTRY(StrictSelector /* strict-selector-match */, 17007, /* DiagArray803 */ 2268, 0, R"()")
DIAG_ENTRY(StringCompare /* string-compare */, 17029, /* DiagArray804 */ 2270, 0, R"()")
DIAG_ENTRY(StringConcatation /* string-concatenation */, 17044, /* DiagArray805 */ 2272, 0, R"()")
DIAG_ENTRY(StringConversion /* string-conversion */, 17065, /* DiagArray806 */ 2274, 0, R"()")
DIAG_ENTRY(StringPlusChar /* string-plus-char */, 17083, /* DiagArray807 */ 2276, 0, R"()")
DIAG_ENTRY(StringPlusInt /* string-plus-int */, 17100, /* DiagArray808 */ 2278, 0, R"()")
DIAG_ENTRY(anonymous_146 /* strlcpy-strlcat-size */, 17116, /* DiagArray809 */ 2280, 0, R"()")
DIAG_ENTRY(StrncatSize /* strncat-size */, 17137, /* DiagArray810 */ 2282, 0, R"()")
DIAG_ENTRY(CXX11WarnSuggestOverrideDestructor /* suggest-destructor-override */, 17150, /* DiagArray811 */ 2286, 0, R"()")
DIAG_ENTRY(CXX11WarnSuggestOverride /* suggest-override */, 17178, /* DiagArray812 */ 2288, 0, R"()")
DIAG_ENTRY(SuperSubClassMismatch /* super-class-method-mismatch */, 17195, /* DiagArray813 */ 2290, 0, R"()")
DIAG_ENTRY(SuspiciousBzero /* suspicious-bzero */, 17223, /* DiagArray814 */ 2292, 0, R"()")
DIAG_ENTRY(SuspiciousMemaccess /* suspicious-memaccess */, 17240, 0, /* DiagSubGroup815 */ 628, R"()")
DIAG_ENTRY(SwiftNameAttribute /* swift-name-attribute */, 17261, /* DiagArray816 */ 2294, 0, R"()")
DIAG_ENTRY(Switch /* switch */, 17282, /* DiagArray817 */ 2308, 0, R"()")
DIAG_ENTRY(SwitchBool /* switch-bool */, 17289, /* DiagArray818 */ 2312, 0, R"()")
DIAG_ENTRY(anonymous_37 /* switch-default */, 17301, 0, 0, R"()")
DIAG_ENTRY(SwitchEnum /* switch-enum */, 17316, /* DiagArray820 */ 2314, 0, R"()")
DIAG_ENTRY(anonymous_263 /* sync-fetch-and-nand-semantics-changed */, 17328, /* DiagArray821 */ 2316, 0, R"()")
DIAG_ENTRY(anonymous_38 /* synth */, 17366, 0, 0, R"()")
DIAG_ENTRY(TargetClonesMixedSpecifiers /* target-clones-mixed-specifiers */, 17372, /* DiagArray823 */ 2318, 0, R"()")
DIAG_ENTRY(TautologicalBitwiseCompare /* tautological-bitwise-compare */, 17403, /* DiagArray824 */ 2320, 0, R"()")
DIAG_ENTRY(TautologicalCompare /* tautological-compare */, 17432, /* DiagArray825 */ 2323, /* DiagSubGroup825 */ 634, R"()")
DIAG_ENTRY(TautologicalConstantCompare /* tautological-constant-compare */, 17453, /* DiagArray826 */ 2326, /* DiagSubGroup826 */ 641, R"()")
DIAG_ENTRY(TautologicalInRangeCompare /* tautological-constant-in-range-compare */, 17483, 0, /* DiagSubGroup827 */ 643, R"()")
DIAG_ENTRY(TautologicalOutOfRangeCompare /* tautological-constant-out-of-range-compare */, 17522, /* DiagArray828 */ 2330, 0, R"()")
DIAG_ENTRY(TautologicalObjCBoolCompare /* tautological-objc-bool-compare */, 17565, /* DiagArray829 */ 2332, 0, R"()")
DIAG_ENTRY(TautologicalOverlapCompare /* tautological-overlap-compare */, 17596, /* DiagArray830 */ 2334, 0, R"()")
DIAG_ENTRY(TautologicalPointerCompare /* tautological-pointer-compare */, 17625, /* DiagArray831 */ 2336, 0, R"()")
DIAG_ENTRY(TautologicalTypeLimitCompare /* tautological-type-limit-compare */, 17654, /* DiagArray832 */ 2339, 0, R"()")
DIAG_ENTRY(TautologicalUndefinedCompare /* tautological-undefined-compare */, 17686, /* DiagArray833 */ 2341, 0, R"()")
DIAG_ENTRY(TautologicalUnsignedCharZeroCompare /* tautological-unsigned-char-zero-compare */, 17717, /* DiagArray834 */ 2344, 0, R"()")
DIAG_ENTRY(TautologicalUnsignedEnumZeroCompare /* tautological-unsigned-enum-zero-compare */, 17757, /* DiagArray835 */ 2346, 0, R"()")
DIAG_ENTRY(TautologicalUnsignedZeroCompare /* tautological-unsigned-zero-compare */, 17797, /* DiagArray836 */ 2348, 0, R"()")
DIAG_ENTRY(TautologicalValueRangeCompare /* tautological-value-range-compare */, 17832, /* DiagArray837 */ 2350, 0, R"()")
DIAG_ENTRY(anonymous_272 /* tcb-enforcement */, 17865, /* DiagArray838 */ 2352, 0, R"()")
DIAG_ENTRY(anonymous_228 /* tentative-definition-incomplete-type */, 17881, /* DiagArray839 */ 2354, 0, R"()")
DIAG_ENTRY(ThreadSafety /* thread-safety */, 17918, 0, /* DiagSubGroup840 */ 646, R"()")
DIAG_ENTRY(ThreadSafetyAnalysis /* thread-safety-analysis */, 17932, /* DiagArray841 */ 2356, 0, R"()")
DIAG_ENTRY(ThreadSafetyAttributes /* thread-safety-attributes */, 17955, /* DiagArray842 */ 2375, 0, R"()")
DIAG_ENTRY(ThreadSafetyBeta /* thread-safety-beta */, 17980, /* DiagArray843 */ 2382, 0, R"()")
DIAG_ENTRY(ThreadSafetyNegative /* thread-safety-negative */, 17999, /* DiagArray844 */ 2384, 0, R"()")
DIAG_ENTRY(ThreadSafetyPrecise /* thread-safety-precise */, 18022, /* DiagArray845 */ 2386, 0, R"()")
DIAG_ENTRY(ThreadSafetyReference /* thread-safety-reference */, 18044, /* DiagArray846 */ 2390, 0, R"()")
DIAG_ENTRY(ThreadSafetyVerbose /* thread-safety-verbose */, 18068, /* DiagArray847 */ 2393, 0, R"()")
DIAG_ENTRY(Trigraphs /* trigraphs */, 18090, /* DiagArray848 */ 2395, 0, R"()")
DIAG_ENTRY(TypeLimits /* type-limits */, 18100, 0, /* DiagSubGroup849 */ 651, R"()")
DIAG_ENTRY(TypeSafety /* type-safety */, 18112, /* DiagArray850 */ 2400, 0, R"()")
DIAG_ENTRY(anonymous_209 /* typedef-redefinition */, 18124, /* DiagArray851 */ 2404, 0, R"()")
DIAG_ENTRY(anonymous_203 /* typename-missing */, 18145, /* DiagArray852 */ 2406, 0, R"()")
DIAG_ENTRY(anonymous_90 /* unable-to-open-stats-file */, 18162, /* DiagArray853 */ 2408, 0, R"()")
DIAG_ENTRY(UnalignedAccess /* unaligned-access */, 18188, /* DiagArray854 */ 2410, 0, R"()")
DIAG_ENTRY(anonymous_266 /* unaligned-qualifier-implicit-cast */, 18205, /* DiagArray855 */ 2412, 0, R"()")
DIAG_ENTRY(UnavailableDeclarations /* unavailable-declarations */, 18239, /* DiagArray856 */ 2414, 0, R"()")
DIAG_ENTRY(UndeclaredSelector /* undeclared-selector */, 18264, /* DiagArray857 */ 2416, 0, R"()")
DIAG_ENTRY(anonymous_112 /* undef */, 18284, /* DiagArray858 */ 2419, 0, R"()")
DIAG_ENTRY(anonymous_113 /* undef-prefix */, 18290, /* DiagArray859 */ 2421, 0, R"()")
DIAG_ENTRY(UndefinedBoolConversion /* undefined-bool-conversion */, 18303, /* DiagArray860 */ 2423, 0, R"()")
DIAG_ENTRY(UndefinedFuncTemplate /* undefined-func-template */, 18329, /* DiagArray861 */ 2426, 0, R"()")
DIAG_ENTRY(anonymous_208 /* undefined-inline */, 18353, /* DiagArray862 */ 2428, 0, R"()")
DIAG_ENTRY(anonymous_206 /* undefined-internal */, 18370, /* DiagArray863 */ 2430, 0, R"()")
DIAG_ENTRY(anonymous_207 /* undefined-internal-type */, 18389, /* DiagArray864 */ 2432, 0, R"()")
DIAG_ENTRY(UndefinedReinterpretCast /* undefined-reinterpret-cast */, 18413, /* DiagArray865 */ 2434, 0, R"()")
DIAG_ENTRY(UndefinedVarTemplate /* undefined-var-template */, 18440, /* DiagArray866 */ 2437, 0, R"()")
DIAG_ENTRY(UnderalignedExceptionObject /* underaligned-exception-object */, 18463, /* DiagArray867 */ 2439, 0, R"()")
DIAG_ENTRY(UnevaluatedExpression /* unevaluated-expression */, 18493, /* DiagArray868 */ 2441, /* DiagSubGroup868 */ 656, R"()")
DIAG_ENTRY(UnguardedAvailability /* unguarded-availability */, 18516, /* DiagArray869 */ 2443, /* DiagSubGroup869 */ 658, R"()")
DIAG_ENTRY(UnguardedAvailabilityNew /* unguarded-availability-new */, 18539, /* DiagArray870 */ 2445, 0, R"()")
DIAG_ENTRY(Unicode /* unicode */, 18566, /* DiagArray871 */ 2447, 0, R"()")
DIAG_ENTRY(anonymous_99 /* unicode-homoglyph */, 18574, /* DiagArray872 */ 2455, 0, R"()")
DIAG_ENTRY(anonymous_98 /* unicode-whitespace */, 18592, /* DiagArray873 */ 2457, 0, R"()")
DIAG_ENTRY(anonymous_100 /* unicode-zero-width */, 18611, /* DiagArray874 */ 2459, 0, R"()")
DIAG_ENTRY(Uninitialized /* uninitialized */, 18630, /* DiagArray875 */ 2461, /* DiagSubGroup875 */ 660, R"()")
DIAG_ENTRY(UninitializedConstReference /* uninitialized-const-reference */, 18644, /* DiagArray876 */ 2469, 0, R"()")
DIAG_ENTRY(UnknownArgument /* unknown-argument */, 18674, /* DiagArray877 */ 2471, 0, R"()")
DIAG_ENTRY(UnknownAssumption /* unknown-assumption */, 18691, /* DiagArray878 */ 2475, 0, R"()")
DIAG_ENTRY(UnknownAttributes /* unknown-attributes */, 18710, /* DiagArray879 */ 2477, 0, R"()")
DIAG_ENTRY(CudaUnknownVersion /* unknown-cuda-version */, 18729, /* DiagArray880 */ 2479, 0, R"()")
DIAG_ENTRY(anonymous_117 /* unknown-directives */, 18750, /* DiagArray881 */ 2482, 0, R"()")
DIAG_ENTRY(anonymous_103 /* unknown-escape-sequence */, 18769, /* DiagArray882 */ 2484, 0, R"()")
DIAG_ENTRY(UnknownPragmas /* unknown-pragmas */, 18793, /* DiagArray883 */ 2486, 0, R"()")
DIAG_ENTRY(UnknownSanitizers /* unknown-sanitizers */, 18809, /* DiagArray884 */ 2507, 0, R"()")
DIAG_ENTRY(UnknownWarningOption /* unknown-warning-option */, 18828, /* DiagArray885 */ 2509, 0, R"()")
DIAG_ENTRY(UnnamedTypeTemplateArgs /* unnamed-type-template-args */, 18851, /* DiagArray886 */ 2513, /* DiagSubGroup886 */ 664, R"()")
DIAG_ENTRY(UnneededInternalDecl /* unneeded-internal-declaration */, 18878, /* DiagArray887 */ 2515, 0, R"()")
DIAG_ENTRY(UnneededMemberFunction /* unneeded-member-function */, 18908, /* DiagArray888 */ 2518, 0, R"()")
DIAG_ENTRY(anonymous_199 /* unqualified-std-cast-call */, 18933, /* DiagArray889 */ 2520, 0, R"()")
DIAG_ENTRY(UnreachableCode /* unreachable-code */, 18959, /* DiagArray890 */ 2522, /* DiagSubGroup890 */ 666, R"()")
DIAG_ENTRY(UnreachableCodeAggressive /* unreachable-code-aggressive */, 18976, 0, /* DiagSubGroup891 */ 670, R"()")
DIAG_ENTRY(UnreachableCodeBreak /* unreachable-code-break */, 19004, /* DiagArray892 */ 2524, 0, R"()")
DIAG_ENTRY(UnreachableCodeFallthrough /* unreachable-code-fallthrough */, 19027, /* DiagArray893 */ 2526, 0, R"()")
DIAG_ENTRY(UnreachableCodeGenericAssoc /* unreachable-code-generic-assoc */, 19056, /* DiagArray894 */ 2528, 0, R"()")
DIAG_ENTRY(UnreachableCodeLoopIncrement /* unreachable-code-loop-increment */, 19087, /* DiagArray895 */ 2530, 0, R"()")
DIAG_ENTRY(UnreachableCodeReturn /* unreachable-code-return */, 19119, /* DiagArray896 */ 2532, 0, R"()")
DIAG_ENTRY(UnsafeBufferUsage /* unsafe-buffer-usage */, 19143, /* DiagArray897 */ 2534, 0, R"()")
DIAG_ENTRY(Unsequenced /* unsequenced */, 19163, /* DiagArray898 */ 2537, 0, R"()")
DIAG_ENTRY(anonymous_79 /* unsupported-abi */, 19175, /* DiagArray899 */ 2540, 0, R"()")
DIAG_ENTRY(UnsupportedAbs /* unsupported-abs */, 19191, /* DiagArray900 */ 2542, 0, R"()")
DIAG_ENTRY(anonymous_188 /* unsupported-availability-guard */, 19207, /* DiagArray901 */ 2545, 0, R"()")
DIAG_ENTRY(UnsupportedCB /* unsupported-cb */, 19238, /* DiagArray902 */ 2547, 0, R"()")
DIAG_ENTRY(anonymous_184 /* unsupported-dll-base-class-template */, 19253, /* DiagArray903 */ 2549, 0, R"()")
DIAG_ENTRY(UnsupportedFPOpt /* unsupported-floating-point-opt */, 19289, /* DiagArray904 */ 2551, 0, R"()")
DIAG_ENTRY(UnsupportedFriend /* unsupported-friend */, 19320, /* DiagArray905 */ 2554, 0, R"()")
DIAG_ENTRY(UnsupportedGPOpt /* unsupported-gpopt */, 19339, /* DiagArray906 */ 2557, 0, R"()")
DIAG_ENTRY(UnsupportedNan /* unsupported-nan */, 19357, /* DiagArray907 */ 2559, 0, R"()")
DIAG_ENTRY(UnsupportedTargetOpt /* unsupported-target-opt */, 19373, /* DiagArray908 */ 2562, 0, R"()")
DIAG_ENTRY(anonymous_194 /* unsupported-visibility */, 19396, /* DiagArray909 */ 2565, 0, R"()")
DIAG_ENTRY(anonymous_201 /* unusable-partial-specialization */, 19419, /* DiagArray910 */ 2567, 0, R"()")
DIAG_ENTRY(Unused /* unused */, 19451, 0, /* DiagSubGroup911 */ 674, R"()")
DIAG_ENTRY(UnusedArgument /* unused-argument */, 19458, 0, 0, R"()")
DIAG_ENTRY(UnusedButSetParameter /* unused-but-set-parameter */, 19474, /* DiagArray913 */ 2569, 0, R"()")
DIAG_ENTRY(UnusedButSetVariable /* unused-but-set-variable */, 19499, /* DiagArray914 */ 2571, 0, R"()")
DIAG_ENTRY(UnusedCommandLineArgument /* unused-command-line-argument */, 19523, /* DiagArray915 */ 2573, 0, R"()")
DIAG_ENTRY(UnusedComparison /* unused-comparison */, 19552, /* DiagArray916 */ 2586, 0, R"()")
DIAG_ENTRY(UnusedConstVariable /* unused-const-variable */, 19570, /* DiagArray917 */ 2588, 0, R"()")
DIAG_ENTRY(UnusedExceptionParameter /* unused-exception-parameter */, 19592, /* DiagArray918 */ 2590, 0, R"()")
DIAG_ENTRY(UnusedFunction /* unused-function */, 19619, /* DiagArray919 */ 2592, /* DiagSubGroup919 */ 685, R"()")
DIAG_ENTRY(UnusedGetterReturnValue /* unused-getter-return-value */, 19635, /* DiagArray920 */ 2594, 0, R"()")
DIAG_ENTRY(UnusedLabel /* unused-label */, 19662, /* DiagArray921 */ 2596, 0, R"()")
DIAG_ENTRY(UnusedLambdaCapture /* unused-lambda-capture */, 19675, /* DiagArray922 */ 2598, 0, R"()")
DIAG_ENTRY(UnusedLocalTypedef /* unused-local-typedef */, 19697, /* DiagArray923 */ 2600, 0, R"()")
DIAG_ENTRY(anonymous_62 /* unused-local-typedefs */, 19718, 0, /* DiagSubGroup924 */ 687, R"()")
DIAG_ENTRY(anonymous_111 /* unused-macros */, 19740, /* DiagArray925 */ 2602, 0, R"()")
DIAG_ENTRY(UnusedMemberFunction /* unused-member-function */, 19754, /* DiagArray926 */ 2604, /* DiagSubGroup926 */ 689, R"()")
DIAG_ENTRY(UnusedParameter /* unused-parameter */, 19777, /* DiagArray927 */ 2606, 0, R"()")
DIAG_ENTRY(UnusedPrivateField /* unused-private-field */, 19794, /* DiagArray928 */ 2608, 0, R"()")
DIAG_ENTRY(UnusedPropertyIvar /* unused-property-ivar */, 19815, /* DiagArray929 */ 2610, 0, R"()")
DIAG_ENTRY(UnusedResult /* unused-result */, 19836, /* DiagArray930 */ 2612, 0, R"()")
DIAG_ENTRY(UnusedTemplate /* unused-template */, 19850, /* DiagArray931 */ 2615, /* DiagSubGroup931 */ 691, R"()")
DIAG_ENTRY(UnusedValue /* unused-value */, 19866, /* DiagArray932 */ 2617, /* DiagSubGroup932 */ 693, R"()")
DIAG_ENTRY(UnusedVariable /* unused-variable */, 19879, /* DiagArray933 */ 2625, /* DiagSubGroup933 */ 697, R"()")
DIAG_ENTRY(anonymous_250 /* unused-volatile-lvalue */, 19895, /* DiagArray934 */ 2627, 0, R"()")
DIAG_ENTRY(UsedButMarkedUnused /* used-but-marked-unused */, 19918, /* DiagArray935 */ 2629, 0, R"()")
DIAG_ENTRY(UserDefinedLiterals /* user-defined-literals */, 19941, /* DiagArray936 */ 2631, 0, R"()")
DIAG_ENTRY(UserDefinedWarnings /* user-defined-warnings */, 19963, /* DiagArray937 */ 2633, 0, R"()")
DIAG_ENTRY(Varargs /* varargs */, 19985, /* DiagArray938 */ 2635, 0, R"()")
DIAG_ENTRY(VariadicMacros /* variadic-macros */, 19993, /* DiagArray939 */ 2639, 0, R"()")
DIAG_ENTRY(anonymous_174 /* vec-elem-size */, 20009, /* DiagArray940 */ 2643, 0, R"()")
DIAG_ENTRY(VectorConversion /* vector-conversion */, 20023, /* DiagArray941 */ 2645, 0, R"()")
DIAG_ENTRY(anonymous_61 /* vector-conversions */, 20041, 0, /* DiagSubGroup942 */ 699, R"()")
DIAG_ENTRY(VexingParse /* vexing-parse */, 20060, /* DiagArray943 */ 2647, 0, R"()")
DIAG_ENTRY(Visibility /* visibility */, 20073, /* DiagArray944 */ 2651, 0, R"()")
DIAG_ENTRY(VLA /* vla */, 20084, /* DiagArray945 */ 2654, /* DiagSubGroup945 */ 701, R"()")
DIAG_ENTRY(VLAExtension /* vla-extension */, 20088, /* DiagArray946 */ 2656, 0, R"()")
DIAG_ENTRY(VoidPointerToEnumCast /* void-pointer-to-enum-cast */, 20102, /* DiagArray947 */ 2658, 0, R"()")
DIAG_ENTRY(VoidPointerToIntCast /* void-pointer-to-int-cast */, 20128, /* DiagArray948 */ 2660, /* DiagSubGroup948 */ 703, R"()")
DIAG_ENTRY(VoidPointerDeref /* void-ptr-dereference */, 20153, /* DiagArray949 */ 2662, 0, R"()")
DIAG_ENTRY(VolatileRegisterVar /* volatile-register-var */, 20174, 0, 0, R"()")
DIAG_ENTRY(WebAssemblyExceptionSpec /* wasm-exception-spec */, 20196, /* DiagArray951 */ 2665, 0, R"()")
DIAG_ENTRY(anonymous_167 /* weak-template-vtables */, 20216, /* DiagArray952 */ 2667, 0, R"()")
DIAG_ENTRY(anonymous_166 /* weak-vtables */, 20238, /* DiagArray953 */ 2669, 0, R"()")
DIAG_ENTRY(WritableStrings /* writable-strings */, 20251, /* DiagArray954 */ 2671, /* DiagSubGroup954 */ 705, R"()")
DIAG_ENTRY(GCCWriteStrings /* write-strings */, 20268, 0, /* DiagSubGroup955 */ 707, R"()")
DIAG_ENTRY(XorUsedAsPow /* xor-used-as-pow */, 20282, /* DiagArray956 */ 2673, 0, R"()")
DIAG_ENTRY(anonymous_271 /* zero-as-null-pointer-constant */, 20298, /* DiagArray957 */ 2677, 0, R"()")
DIAG_ENTRY(ZeroLengthArray /* zero-length-array */, 20328, /* DiagArray958 */ 2679, 0, R"()")
#endif // DIAG_ENTRY


#ifdef GET_CATEGORY_TABLE
CATEGORY("", DiagCat_None)
CATEGORY("Lexical or Preprocessor Issue", DiagCat_Lexical_or_Preprocessor_Issue)
CATEGORY("Semantic Issue", DiagCat_Semantic_Issue)
CATEGORY("Lambda Issue", DiagCat_Lambda_Issue)
CATEGORY("Parse Issue", DiagCat_Parse_Issue)
CATEGORY("ARC Semantic Issue", DiagCat_ARC_Semantic_Issue)
CATEGORY("ARC and @properties", DiagCat_ARC_and__properties)
CATEGORY("ARC Casting Rules", DiagCat_ARC_Casting_Rules)
CATEGORY("ARC Parse Issue", DiagCat_ARC_Parse_Issue)
CATEGORY("ARC Weak References", DiagCat_ARC_Weak_References)
CATEGORY("ARC Restrictions", DiagCat_ARC_Restrictions)
CATEGORY("OpenMP Issue", DiagCat_OpenMP_Issue)
CATEGORY("Inline Assembly Issue", DiagCat_Inline_Assembly_Issue)
CATEGORY("AST Deserialization Issue", DiagCat_AST_Deserialization_Issue)
CATEGORY("Modules Issue", DiagCat_Modules_Issue)
CATEGORY("Coroutines Issue", DiagCat_Coroutines_Issue)
CATEGORY("Concepts Issue", DiagCat_Concepts_Issue)
CATEGORY("Dependency Directive Source Scanner Issue", DiagCat_Dependency_Directive_Source_Scanner_Issue)
CATEGORY("Backend Issue", DiagCat_Backend_Issue)
CATEGORY("SourceMgr Reported Issue", DiagCat_SourceMgr_Reported_Issue)
CATEGORY("Related Result Type Issue", DiagCat_Related_Result_Type_Issue)
CATEGORY("AST Serialization Issue", DiagCat_AST_Serialization_Issue)
CATEGORY("Nullability Issue", DiagCat_Nullability_Issue)
CATEGORY("Generics Issue", DiagCat_Generics_Issue)
CATEGORY("User-Defined Issue", DiagCat_User_Defined_Issue)
CATEGORY("Refactoring Invocation Issue", DiagCat_Refactoring_Invocation_Issue)
CATEGORY("VTable ABI Issue", DiagCat_VTable_ABI_Issue)
CATEGORY("Value Conversion Issue", DiagCat_Value_Conversion_Issue)
CATEGORY("Documentation Issue", DiagCat_Documentation_Issue)
CATEGORY("ARC Retain Cycle", DiagCat_ARC_Retain_Cycle)
CATEGORY("Deprecations", DiagCat_Deprecations)
CATEGORY("Format String Issue", DiagCat_Format_String_Issue)
CATEGORY("Cocoa API Issue", DiagCat_Cocoa_API_Issue)
CATEGORY("#pragma message Directive", DiagCat__pragma_message_Directive)
CATEGORY("Instrumentation Issue", DiagCat_Instrumentation_Issue)
CATEGORY("Unused Entity Issue", DiagCat_Unused_Entity_Issue)
#endif // GET_CATEGORY_TABLE

