local ffi = require("ffi")
local bit = require("bit")
-- -- Require submodules
ffi_util = require("sparrow.ffi_util")


-- Create a class to manage a Bitset
local Bitset = {}
Bitset.__index = Bitset


function Bitset.new(size, ctype, auto_gc)

    if auto_gc == nil then
        auto_gc = true
    end

    local self = setmetatable({}, Bitset)
    self.size = size
    local ctype = "uint8_t"

    local malloc_size = (size + 7) / 8
    if auto_gc == true then
        self.ptr = ffi_util.cast(ffi.gc(ffi.C.malloc(malloc_size), ffi.C.free), ctype)
    else
        self.ptr = ffi_util.cast(ffi.C.malloc(malloc_size), ctype)
    end

    -- Cleanup memory
    if auto_gc then
        Bitset.free = function () end
    else
        Bitset.free = function ()
            ffi.C.free(self.ptr)
            self.ptr = nil
            Bitset.free = function ()  end
        end
    end

    return self
end

function Bitset.view(ptr, size, ctype)
    local self = setmetatable({}, Bitset)
    self.size = size
    self.ctype = ctype 
    self.ptr = ptr
    Bitset.free = function () end
    return self
end

-- Implement the __index metamethod for reading
function Bitset:__index(key)
    if type(key) == "number" then
        if key < 0 or key >= self.size then
            error("Index out of bounds")
        end
        
        local byte = key / 8
        local bit = key % 8
        return bit.band(ffi.cast("uint8_t", self.ptr[byte]), bit.lshift(1, bit)) ~= 0
    else
        -- Fallback to accessing methods or properties
        return rawget(Bitset, key)
    end
end

-- Implement the __newindex metamethod for writing
function Bitset:__newindex(key, value)
    if type(key) == "number" then
        if key < 0 or key >= self.size then
            error("Index out of bounds")
        end
        local byte = key / 8
        local bit_index = key % 8
        if value then
            self.ptr[byte] = bit.bor(ffi.cast("uint8_t", self.ptr[byte]), bit.lshift(1, bit_index))
        else
            self.ptr[byte] = bit.band(ffi.cast("uint8_t", self.ptr[byte]), bit.bnot(bit.lshift(1, bit_index)))
        end
    else
        rawset(self, key, value) -- Allow setting other properties
    end
end

-- get method
function Bitset:get(key)
    local byte = key / 8
    local bit_index = key % 8
    return bit.band(ffi.cast("uint8_t", self.ptr[byte]), bit.lshift(1, bit_index)) ~= 0
end

-- print Bitset
function Bitset:print()
    for i = 0, self.size - 1 do
        print(self:get(i))
    end
end


return {
    Bitset = Bitset
}