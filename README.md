# bonefinity
(should shrink over time where overlaps Standard Library) This is a 2.5D game engine that uses 3D physics, and some 3D drawing.

## Known Issues:
* remove commented code (FDist?, Dist?)
* remove uses of Convert::ToString and only use std::to_string (eliminate RString_ToString)
* eliminate use of frameworkdummy
* (base.*) deprecate char* methods: trunc, Upperstring, (many more) and Crop IntOfString DoubleOfString FloatOfString IsExt ExistsAt IndexOf
* use boost in scope of `catch (...)`: `std::cerr << "Unexpected exception, diagnostic information follows:\n" << current_exception_diagnostic_information();`
