# import_scrambler
an import scrambler because [mephistopheles](https://github.com/y5) had issues with other scramblers :sunglasses:

this library on its own doesn't really protect you from anything but can be pretty useful against skids in combination with xor'ing your strings or just replacing all strings with compile time hashes

# ida vs import_scrambler
when building in release mode and not loading the debug symbols ida fails to generate pseudocode for the call to my library.

![call analysis failed][call_failed]

if you disassemble a debug build with the symbols you can get normal pseudocode like you are used to.

normal winapi calls:

![native call][native_call]

vs

import_scrambler winapi calls:

![scrambled call][scrambled_call]

and if you didn't think this library would hide the imports after reading the name of this repo you should get yourself checked out and afterwards look at this picture:

![ida imports][import]

[native_call]: http://maxxyz.u.catgirlsare.sexy/pqJ4.png
[scrambled_call]: http://maxxyz.u.catgirlsare.sexy/pzjm.png
[call_failed]: http://maxxyz.u.catgirlsare.sexy/Ty3L.png
[import]: http://maxxyz.u.catgirlsare.sexy/WvQr.png