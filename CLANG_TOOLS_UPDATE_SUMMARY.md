# Clang Tools Configuration Update Summary

**Versions:**
- clang-format: 21.1.8
- clang-tidy: 21.1.8

## .clang-format Changes

### New Settings Added (Key Items)

#### 1. **Alignment Options** - Now use structured sub-options
Your old config had simple boolean values:
```yaml
AlignConsecutiveAssignments: true
AlignConsecutiveBitFields: true
AlignConsecutiveDeclarations: true
AlignConsecutiveMacros: true
```

New format provides fine-grained control:
```yaml
AlignConsecutiveAssignments:
  Enabled:         true
  AcrossEmptyLines: false  # Don't align across blank lines
  AcrossComments:  false   # Don't align across comments
  AlignCompound:   false   # Don't align compound assignments differently
  PadOperators:    true    # Pad operators for alignment
```

**Recommendation:** Keep `AcrossEmptyLines: false` and `AcrossComments: false` for readability.

#### 2. **AlignArrayOfStructures** (NEW)
- Options: `None`, `Left`, `Right`
- Default: `None`
- **Recommendation:** Keep as `None` unless you frequently use array-of-struct initializers

#### 3. **AlignConsecutiveShortCaseStatements** (NEW)
```yaml
AlignConsecutiveShortCaseStatements:
  Enabled:         false
  AlignCaseArrows: false  # For pattern matching (C++, not relevant yet)
  AlignCaseColons: false
```
**Recommendation:** Leave disabled unless you have many short case statements

#### 4. **AlignTrailingComments** - Now structured
Old: `AlignTrailingComments: true`
New:
```yaml
AlignTrailingComments:
  Kind:            Always
  OverEmptyLines:  0  # Don't align comments separated by empty lines
```

#### 5. **AllowBreakBeforeNoexceptSpecifier** (NEW)
- Options: `Never`, `OnlyWithParen`, `Always`
- Set to: `Never`
- **Recommendation:** Keep as `Never` for consistency

#### 6. **BreakBeforeConceptDeclarations** (NEW)
- Options: `Never`, `Allowed`, `Always`
- Set to: `Always` (C++20 concepts)
- **Recommendation:** Keep as `Always` for C++20 code

#### 7. **BreakBinaryOperations** (NEW)
- Options: `Never`, `RespectPrecedence`, `OnePerLine`
- Set to: `Never`
- **Recommendation:** Consider `RespectPrecedence` for complex expressions

#### 8. **BracedInitializerIndentWidth** (NEW)
- Default: `-1` (use IndentWidth)
- **Recommendation:** Keep as `-1` for consistency

#### 9. **PackConstructorInitializers** (NEW - replaces old option)
- Options: `Never`, `BinPack`, `CurrentLine`, `NextLine`, `NextLineOnly`
- Set to: `CurrentLine`
- **Recommendation:** Consider `NextLine` if you prefer initializers on separate line

#### 10. **EmptyLineAfterAccessModifier** & **EmptyLineBeforeAccessModifier** (NEW)
```yaml
EmptyLineAfterAccessModifier: Never
EmptyLineBeforeAccessModifier: LogicalBlock
```
**Recommendation:** Good defaults for class formatting

#### 11. **KeepEmptyLines** (NEW - replaces KeepEmptyLinesAtTheStartOfBlocks)
Old: `KeepEmptyLinesAtTheStartOfBlocks: true`
New:
```yaml
KeepEmptyLines:
  AtEndOfFile:     false
  AtStartOfBlock:  true
  AtStartOfFile:   true
```
**Recommendation:** Set `AtEndOfFile: false` to remove trailing newlines

#### 12. **IndentRequiresClause** & **RequiresClausePosition** (NEW - C++20)
```yaml
IndentRequiresClause: true
RequiresClausePosition: OwnLine
```
**Recommendation:** Keep for proper C++20 concepts formatting

#### 13. **InsertBraces** (NEW)
- Adds braces to single-statement control flow
- Default: `false`
- **Recommendation:** Keep `false` - use clang-tidy for enforcement instead

#### 14. **InsertNewlineAtEOF** (NEW)
- Default: `false`
- **Recommendation:** Consider setting to `true` for POSIX compliance

#### 15. **IntegerLiteralSeparator** (NEW)
```yaml
IntegerLiteralSeparator:
  Binary:          0  # Use ' separator every N digits
  Decimal:         0  # 0 = disabled
  Hex:             0
```
**Recommendation:** Enable for large constants (e.g., `Binary: 4`, `Decimal: 3`, `Hex: 4`)
Example: `1000000` → `1'000'000`, `0xDEADBEEF` → `0xDEAD'BEEF`

#### 16. **LambdaBodyIndentation** (NEW)
- Options: `Signature`, `OuterScope`
- Set to: `Signature`
- **Recommendation:** Keep `Signature` for consistency with function indentation

#### 17. **LineEnding** (NEW)
- Options: `LF`, `CRLF`, `DeriveLF`, `DeriveCRLF`
- Set to: `DeriveLF`
- **Recommendation:** Keep `DeriveLF` for cross-platform consistency

#### 18. **QualifierAlignment** (NEW)
- Options: `Leave`, `Left`, `Right`, `Custom`
- Set to: `Leave`
- **Recommendation:** Keep `Leave` or use `Left` for `const int *` style

#### 19. **ReferenceAlignment** (NEW)
- Options: `Pointer`, `Left`, `Right`, `Middle`
- Set to: `Pointer` (follows PointerAlignment)
- **Recommendation:** Good with your `PointerAlignment: Middle`

#### 20. **RemoveBracesLLVM**, **RemoveParentheses**, **RemoveSemicolon** (NEW)
- All set to `false`/`Leave`
- **Recommendation:** Keep disabled - too aggressive

#### 21. **SeparateDefinitionBlocks** (NEW)
- Options: `Leave`, `Always`, `Never`
- Set to: `Leave`
- **Recommendation:** Consider `Always` to enforce blank lines between definitions

#### 22. **ShortNamespaceLines** (NEW)
- Threshold for single-line namespaces
- Default: `1`

#### 23. **SortIncludes** - Now structured
Old: `SortIncludes: CaseInsensitive`
New:
```yaml
SortIncludes:
  Enabled:         true
  IgnoreCase:      false
```
**Recommendation:** Keep as shown (case-sensitive sorting)

#### 24. **SortUsingDeclarations** - New options
Old: `SortUsingDeclarations: true`
New: `SortUsingDeclarations: LexicographicNumeric`
Options: `Never`, `Lexicographic`, `LexicographicNumeric`

#### 25. **SpaceAroundPointerQualifiers** (NEW)
- Options: `Default`, `Before`, `After`, `Both`
- Controls spacing around `const`, `volatile` in pointer types
- **Recommendation:** Keep `Default`

#### 26. **SpaceBeforeParensOptions** (NEW - replaces SpaceBeforeParens in some cases)
Fine-grained control over spacing before parentheses:
```yaml
SpaceBeforeParensOptions:
  AfterControlStatements: true
  AfterFunctionDeclarationName: false
  AfterRequiresInClause: false
  # ... etc
```

#### 27. **SpacesInLineCommentPrefix** (NEW)
```yaml
SpacesInLineCommentPrefix:
  Minimum:         1
  Maximum:         -1  # unlimited
```
Controls spacing after `//`

#### 28. **SpacesInParens** (NEW - replaces SpacesInParentheses)
Old: `SpacesInParentheses: false`
New: `SpacesInParens: Never` + `SpacesInParensOptions` for fine control

### Deprecated/Removed Options
These were in your old config but are deprecated:
- `AlwaysBreakTemplateDeclarations: Yes` → `BreakTemplateDeclarations: Yes`
- `ConstructorInitializerAllOnOneLineOrOnePerLine` → Removed
- `DeriveLineEnding` → Replaced by `LineEnding: DeriveLF`

---

## .clang-tidy Changes

Your original .clang-tidy was minimal (only `Checks:` line). The new version adds:

### New Top-Level Options

#### 1. **WarningsAsErrors** (NEW)
```yaml
WarningsAsErrors: ''
```
- Set to specific checks to treat as errors: `'bugprone-*,cert-*'`
- **Recommendation:** Leave empty initially, add specific checks gradually

#### 2. **HeaderFileExtensions** & **ImplementationFileExtensions** (NEW)
```yaml
HeaderFileExtensions:
  - ''
  - h
  - hh
  - hpp
  - hxx
ImplementationFileExtensions:
  - c
  - cc
  - cpp
  - cxx
```
**Recommendation:** Covers standard extensions

#### 3. **HeaderFilterRegex** (NEW)
- Regex to filter which headers to check
- Default: `''` (check all)
- **Recommendation:** Set to `'.*'` to check all project headers, or `'hinder/.*'` for project-only

#### 4. **ExcludeHeaderFilterRegex** (NEW)
- Regex to exclude headers
- **Recommendation:** Use to exclude third-party: `'.*/third_party/.*'`

#### 5. **FormatStyle** (NEW)
- Options: `none`, `file`, `llvm`, etc.
- Set to: `none` (uses .clang-format)
- **Recommendation:** Keep as `none` to use your .clang-format file

#### 6. **SystemHeaders** (NEW)
- Check system headers
- Default: `false`
- **Recommendation:** Keep `false` (don't check system headers)

#### 7. **CheckOptions** (NEW)
Huge section with per-check configuration. Common ones included in new config:

**Modernize checks:**
- `modernize-use-std-format.FormatHeader: '<format>'`
- `modernize-use-std-print.PrintHeader: '<print>'`
- `modernize-use-nodiscard.ReplacementString: '[[nodiscard]]'`

**Readability checks:**
- `readability-function-cognitive-complexity.Threshold: '25'`
- `readability-identifier-length.MinimumVariableNameLength: '3'`

**Recommendation:** Start with minimal CheckOptions, add as needed

### Enhanced Check Categories
The new version adds:
- `clang-diagnostic-*` (compiler warnings as checks)

---

## Key Recommendations

### High Priority Changes

1. **Update AlignConsecutive* to structured format** - Required for clang-format 21
2. **Set `InsertNewlineAtEOF: true`** - POSIX compliance
3. **Enable `IntegerLiteralSeparator`** - Improves readability of large constants
4. **Add `HeaderFilterRegex: '.*'`** to .clang-tidy - Check all headers
5. **Consider `PackConstructorInitializers: NextLine`** - Better readability

### Medium Priority

6. **`BreakBinaryOperations: RespectPrecedence`** - Better for complex expressions
7. **`SeparateDefinitionBlocks: Always`** - Consistent spacing
8. **`QualifierAlignment: Left`** - If you prefer `const int *` over `int const *`
9. **Add specific `CheckOptions` as needed** - Customize check behavior

### Low Priority / As Needed

10. **`AlignArrayOfStructures`** - Only if you use array-of-struct initializers
11. **`WarningsAsErrors`** in .clang-tidy - After stabilizing
12. **`InsertBraces: true`** - Only if you want automatic brace insertion
13. **Custom `CheckOptions`** - Fine-tune specific checks

---

## Testing the New Configuration

1. **Backup current files:**
   ```bash
   cp .clang-format .clang-format.old
   cp .clang-tidy .clang-tidy.old
   ```

2. **Try new configs:**
   ```bash
   cp .clang-format.new .clang-format
   cp .clang-tidy.new .clang-tidy
   ```

3. **Test on timestamp files:**
   ```bash
   clang-format -i src/core/timestamp.cpp
   clang-format -i include/hinder/core/timestamp.h
   git diff  # Review changes
   ```

4. **Run clang-tidy:**
   ```bash
   clang-tidy src/core/timestamp.cpp -- -std=c++20 -I./include
   ```

5. **If satisfied, format entire codebase:**
   ```bash
   find src include -name "*.cpp" -o -name "*.h" | xargs clang-format -i
   ```

---

## VS Code Integration

### clang-format

Install extension: `xaver.clang-format`

Add to `.vscode/settings.json`:
```json
{
  "editor.formatOnSave": true,
  "C_Cpp.formatting": "clangFormat",
  "clang-format.executable": "/usr/bin/clang-format",
  "clang-format.style": "file"
}
```

### clang-tidy

Install extension: `notskm.clang-tidy`

Add to `.vscode/settings.json`:
```json
{
  "clang-tidy.executable": "/usr/bin/clang-tidy",
  "clang-tidy.buildPath": "${workspaceFolder}/build",
  "clang-tidy.lintOnSave": true,
  "clang-tidy.compileCommandsPath": "${workspaceFolder}/build"
}
```

Make sure to generate `compile_commands.json`:
```bash
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
```
