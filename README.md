# bonefinity
(should shrink over time where overlaps Standard Library) This is a 2.5D game engine that uses 3D physics, and some 3D drawing.

## Compiling
* Make sure Code::Blocks is installed (on GNU+Linux systems, install the `codeblocks` package; on Windows, download & install the program from codeblocks.org)
* Make sure the Boost library is in your path on your platform
* Open "base.workspace" (or "Base.cbp" in your own workspace) using Code::Blocks
* Click "Build," "Build and run"
* IF says missing many `h` and `cpp` files even though they are part of the project:
  * Settings, Compiler, Search Directories, Compiler, Add
* IF undefined reference to boost:
  * ensure Code::Blocks has found boost during compile: make sure boost libraries and headers are installed; if still not found, add boost's lib directory (containing lib files if on Windows) to the linker search paths in Settings, Compiler, Search directories, Linker

## Known Issues:
* remove commented code (FDist?, Dist?)
* remove uses of Convert::ToString and only use std::to_string (eliminate RString_ToString)
* eliminate use of frameworkdummy
* (base.*) deprecate char* methods: trunc, Upperstring, (many more) and Crop IntOfString DoubleOfString FloatOfString IsExt ExistsAt IndexOf
* use boost in scope of `catch (...)`: `std::cerr << "Unexpected exception, diagnostic information follows:\n" << current_exception_diagnostic_information();`


## Developer Notes
* (already done) IF undefined reference to `boost::system::generic_category()`:
  * add `-lboost_system` to linker flags in Settings, Compiler, Linker settings, Other linker options
  * IF undefined reference to `boost::filesystem::path::operator/=(boost::filesystem::path const&)`:
    * also add -lboost_filesystem
    (if you add that without -lboost_system you may get: undefined reference to symbol '_ZNSboost6system16generic_categoryEv')
