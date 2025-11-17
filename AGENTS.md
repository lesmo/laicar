# LAICar: Lesmo's AI Car

This is a project forked from SunnyPilot, an open source driver assistance system for comma.ai's comma 3X.

Unless told otherwise, assume the vehicle is a Volkswagen GLI 2021.

## Build Process

Always activate the .venv/bin/activate Python virtual environment for `scons` to be available.

## Python Scripts

Always activate the .venv/bin/activate script before running any python scripts.

## Code Generation

### Hooks and Interventions

You must attempt to avoid making major code changes to the original codebase outside of the laicar/ directory. Instead, you should try to keep new code inside of the laicar/ directory.

To achieve this, we may use decorators to add code before and after the original function.

### Overriding C/C++ Functions

To override C/C++ functions from the original codebase while keeping laicar-specific code in `laicar/`:

1. **Create the override file** in the corresponding `laicar/` directory structure:
   - Example: To override `selfdrive/ui/qt/util.cc`, create `laicar/ui/qt/util.cc`
   - Include the original header: `#include "selfdrive/ui/qt/util.h"`
   - Implement only the functions you want to override with the same signature

2. **Create or update the SConscript** in the laicar subdirectory:
   - Example: `laicar/ui/SConscript`
   - Export the source files in a list variable (e.g., `laicar_qt_util`)
   ```python
   laicar_qt_util = [
     "qt/util.cc",
   ]
   Export('laicar_qt_util')
   ```

3. **Modify the original SConscript** to include laicar overrides:
   - Import the laicar source list
   - Place laicar files **FIRST** in the Library() call to give them priority
   ```python
   # Import laicar overrides
   laicar_qt_util = []
   SConscript(['../../laicar/ui/SConscript'])
   Import('laicar_qt_util')

   # Build library with laicar files FIRST
   qt_util = qt_env.Library("qt_util",
     [f"#laicar/ui/{src}" for src in laicar_qt_util] +
     ["#selfdrive/ui/qt/api.cc", "#selfdrive/ui/qt/util.cc"] +
     sp_qt_util,
     LIBS=base_libs)
   ```

4. **Mark the original function as weak** to allow overriding:
   - In the original source file, add `__attribute__((weak))` before the function
   ```cpp
   __attribute__((weak)) QString getBrand() {
     return QObject::tr("sunnypilot");
   }
   ```

5. **Rebuild the target**:
   ```bash
   . .venv/bin/activate
   scons -c <target>
   scons -j$(nproc) <target>
   ```

**How it works**: Static libraries combine object files, and the linker prefers (1) strong symbols over weak symbols, and (2) earlier symbols in link order. By placing laicar files first with strong symbols and marking the base version weak, the linker selects the laicar implementation.