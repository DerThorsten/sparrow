local ffi = require("ffi")


--
ffi_util = require("sparrow.ffi_def")



-- Cast a pointer to a type
local function cast(ptr, ctype)
    return ffi.cast(ffi.typeof(ctype .. "*"), ptr)
end


return {
    cast = cast
}