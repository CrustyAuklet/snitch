version = meson.project_version()
vers = version.split('.')

git_hash = run_command(find_program('git'),'log','-1','--format=%h'
                      ,check:true).stdout().strip()

conf_data = configuration_data({
  'PROJECT_VERSION' : version,
  'PROJECT_VERSION_MAJOR' : vers[0],
  'PROJECT_VERSION_MINOR' : vers[1],
  'PROJECT_VERSION_PATCH' : vers[2],

  'SNITCH_FULL_VERSION' : version + '.' + git_hash,

  'SNITCH_MAX_TEST_CASES'        : get_option('max_test_cases'),
  'SNITCH_MAX_NESTED_SECTIONS'   : get_option('max_nested_sections'),
  'SNITCH_MAX_EXPR_LENGTH'       : get_option('max_expr_length'),
  'SNITCH_MAX_MESSAGE_LENGTH'    : get_option('max_message_length'),
  'SNITCH_MAX_TEST_NAME_LENGTH'  : get_option('max_test_name_length'),
  'SNITCH_MAX_TAG_LENGTH'        : get_option('max_tag_length'),
  'SNITCH_MAX_CAPTURES'          : get_option('max_captures'),
  'SNITCH_MAX_CAPTURE_LENGTH'    : get_option('max_capture_length'),
  'SNITCH_MAX_UNIQUE_TAGS'       : get_option('max_unique_tags'),
  'SNITCH_MAX_COMMAND_LINE_ARGS' : get_option('max_command_line_args'),

  'SNITCH_DEFINE_MAIN'           : get_option('define_main').to_int(),
  'SNITCH_WITH_EXCEPTIONS'       : get_option('with_exceptions').to_int(),
  'SNITCH_WITH_TIMINGS'          : get_option('with_timings').to_int(),
  'SNITCH_WITH_SHORTHAND_MACROS' : get_option('with_shorthand_macros').to_int(),
  'SNITCH_DEFAULT_WITH_COLOR'    : get_option('default_with_color').to_int(),
  'SNITCH_CREATE_HEADER_ONLY'    : get_option('create_header_only').to_int(),
  'SNITCH_CREATE_LIBRARY'        : get_option('create_library').to_int(),
})

conf_file = configure_file(
  input: meson.project_source_root() / 'include/snitch/snitch_config.hpp.config',
  output: 'snitch_config.hpp',
  format: 'cmake',
  configuration: conf_data,
  install_dir: 'include/snitch'
)

snitch_all = custom_target(
  output: 'snitch_all.hpp',
  build_by_default: get_option('create_header_only'),
  depend_files : [conf_file, headers, main],
  command: [find_program('python3'), make_snitch_all,
            meson.project_source_root(), meson.project_build_root()],
  depfile: 'snitch_all.d',
  install: true,
  install_dir: 'include/snitch'
)