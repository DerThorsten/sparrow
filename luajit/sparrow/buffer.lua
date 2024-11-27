local ffi = require("ffi")

-- -- Require submodules
ffi_util = require("sparrow.ffi_util")


-- Create a class to manage a buffer
local Buffer = {}
Buffer.__index = Buffer


function Buffer.new(size, ctype, auto_gc)


    if auto_gc == nil then
        auto_gc = true
    end

    local self = setmetatable({}, Buffer)
    self.size = size
    self.ctype = ctype

    self.owner = true
    self.auto_gc = auto_gc
    self.parent = nil

    local malloc_size = size * ffi.sizeof(ctype)
    if auto_gc == true then
        self.ptr = ffi_util.cast(ffi.gc(ffi.C.malloc(malloc_size), ffi.C.free), ctype)
    else
        self.ptr = ffi_util.cast(ffi.C.malloc(malloc_size), ctype)
    end

    -- Cleanup memory
    if auto_gc then
        Buffer.free = function () end
    else
        Buffer.free = function ()
            ffi.C.free(self.ptr)
            self.ptr = nil
            Buffer.free = function ()  end
        end
    end

    return self
end

function Buffer.view(ptr, size, ctype, parent)
    local self = setmetatable({}, Buffer)
    self.owner = false
    self.auto_gc = false
    self.parent = parent
    self.size = size
    self.ctype = ctype 
    self.ptr = ptr
    Buffer.free = function () end
    return self
end


function Buffer.slice(self, start, size)
    return Buffer.view(self.ptr + start, size, self.ctype, self)
end

-- Implement the __index metamethod for reading
function Buffer:__index(key)
    if type(key) == "number" then
        if key < 0 or key >= self.size then
            error("Index out of bounds")
        end
        return self.ptr[key]
    else
        -- Fallback to accessing methods or properties
        return rawget(Buffer, key)
    end
end

-- slice 



-- get method
function Buffer:get(key)
    return self.ptr[key]
end

-- Implement the __newindex metamethod for writing
function Buffer:__newindex(key, value)
    if type(key) == "number" then
        if key < 0 or key >= self.size then
            error("Index out of bounds")
        end
        self.ptr[key] = value
    else
        rawset(self, key, value) -- Allow setting other properties
    end
end

-- print buffer
function Buffer:print()
    print("Printing buffer", self.size,self.ctype)
    for i = 0, self.size - 1 do
        print(self.ptr[i])
    end
end


return {
    Buffer = Buffer
}