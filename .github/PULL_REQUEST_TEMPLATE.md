## =================================================================
# PR Guidelines

1. All PRs must land into the `develop` branch (not `main`.) All PRs whose base is set to other than `develop` will be rejected. Please check your base branch above.
2. Automatic code formatting checks are run. Please make sure your code is formatted using the `.clang-format` file provided. You may consider running this at the top directory: `find -E . -type f ! -regex '.*/(extern|build)/.*' -regex '.*\.(cpp|h)' -exec clang-format -style=file -i {} \;`
3. Unit tests must pass for a PR to be merged in.
## Please remove after checking ====================================
