# Get the name of the top-level project
get_filename_component(ROOT_PROJECT ${PROJECT_NAME} NAME)

# Pass the macro value to the compiler
target_compile_definitions(${ROOT_PROJECT} PRIVATE DEFAULT_LOG_FILE_NAME="Log_${ROOT_PROJECT}.txt" CONFIG_FILE_NAME="Config_${ROOT_PROJECT}.yaml")