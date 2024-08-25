# ---- In-source guard ----

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  message(
      FATAL_ERROR
      "In-source builds are not supported. "
      "Please read the BUILDING document before trying to build this project. "
      "You may need to delete 'CMakeCache.txt' and 'CMakeFiles/' first."
  )
endif()

function(enable_warnings warning_target)
  if(NOT TARGET ${warning_target})
    message(WARNING "multipleint warnings: ${warning_target} is not a target.")
    return()
  endif()

  target_compile_options(
    ${warning_target}
    PRIVATE $<$<CXX_COMPILER_ID:GNU>:
            -Wall
            -Wextra
            -pedantic
            -pedantic-errors
            # Warnings that are not enabled by -Wall/-Wextra See
            # https://kristerw.blogspot.com/2017/09/useful-gcc-warning-
            # options-not- enabled.html
            -Wunknown-pragmas
            -Wundef
            -Wold-style-cast # warn for c-style casts (e.g. `(int) 3.0`)
            -Wno-useless-cast # deactivated because of moc
            -Wdisabled-optimization
            -Wstrict-overflow=2
            -Winit-self
            -Wpointer-arith
            -Wduplicated-cond
            -Wdouble-promotion
            -Wno-comment
            -Wshadow # warn the user if a variable declaration shadows one from
                     # a  parent context
            -Wduplicated-branches
            -Wrestrict
            -Wnull-dereference # warn if a null dereference is detected
            -Wlogical-op
            -Wunsafe-loop-optimizations
            -Wno-error=unsafe-loop-optimizations
            -Wformat=2
            -Wmissing-field-initializers
            -Wconversion 
            -Wsign-conversion
            >
            $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
            -Wall
            -Wextra
            -pedantic
            -Wno-missing-braces
            -Wdocumentation # Warns about doxygen variable  name mismatches,
                            # etc.
            -Wno-gnu-zero-variadic-macro-arguments # false positive for qCDebug
            >
            $<$<CXX_COMPILER_ID:MSVC>:
            /W3>
  )
endfunction()
