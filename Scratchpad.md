    # Emscripten
    # Avoid "undefined symbol" errors, see: https://stackoverflow.com/a/37869647
    target_link_options(stub_test PRIVATE "--bind")
