project_add_compile_options(ALL
        -mcpu=cortex-m3
        -mthumb
        -mfloat-abi=soft

        --specs=nano.specs

        -masm-syntax-unified
)


project_add_link_options(ALL
        -mcpu=cortex-m3
        -specs=nano.specs
)