

local buffer = require("sparrow.buffer")

local BaseLayout = {}
BaseLayout.__index = BaseLayout


local SubRange = {}
SubRange.__index = SubRange

function SubRange:new(parent, start, size)
    local instance = {}
    setmetatable(instance, self)
    instance.parent = parent
    instance.start = start
    instance.size = size
    return instance
end

function SubRange:get(key)
    return self.parent:get(self.start + key)
end

function SubRange:__index(key)
    if type(key) == "number" then
        return self.parent:get(self.start + key)
    end
    return rawget(self, key) or SubRange[key]
end







function BaseLayout:inherit()
    local Subclass = {}
    Subclass.__index = Subclass
    -- Note how `self` in this case is the parent class, as we call the method like `SomeClass:inherit()`.
    setmetatable(Subclass, self)
    return Subclass
end

function BaseLayout:initialize()
    error("this class cannot be initialized")
end

function BaseLayout:new(...)
    local instance = {}
    setmetatable(instance, self)
    self.initialize(instance, ...)
    return instance
end

function BaseLayout:__index(key)
    if type(key) == "number" then
        return self:get(key)
    end
    return rawget(self, key) or BaseLayout[key]
end


function BaseLayout:_subrange(start, size)
    return SubRange:new(self, start, size)
end

-- print BaseLayout
function BaseLayout:print()
    -- class name 

    for i = 0, self.size - 1 do
        print(self:get(i))
    end
end




local NumericLayout = BaseLayout:inherit()

function NumericLayout:__index(key)
    if type(key) == "number" then
        return self:get(key)
    end
    return rawget(self, key) or NumericLayout[key] or BaseLayout[key]
end

function NumericLayout:initialize(data, bitmap)
    self.data = data
    self.ptr = data.ptr
    self.bitmap = bitmap
    self.size = data.size
    self.offset = 0
    NumericLayout.set_functions(self)
end

function NumericLayout:set_functions()
    if self.bitmap == nil then
        function NumericLayout:get(key)
            return self.ptr[key]
        end
        function NumericLayout:_subrange(start, size)
            return SubRange:new(self, start, size)
        end
    else
        function NumericLayout:get(key)
            return self.bitmap:get(key) and self.ptr[key] or nil
        end
        function NumericLayout:_subrange(start, size)
            return SubRange:new(self, start, size)
        end
    end
end

local PtrWithSize = {}
PtrWithSize.__index = PtrWithSize

function PtrWithSize:new(ptr, size)
    local instance = {}
    setmetatable(instance, self)
    instance.ptr = ptr
    instance.size = size
    return instance
end

function PtrWithSize:__index(key)
    if type(key) == "number" then
        return self.ptr[key]
    end
    return rawget(self, key) or PtrWithSize[key]
end

-- get method
function PtrWithSize:get(key)
    return self.ptr[key]
end



return {
    NumericLayout = NumericLayout,
    print_array = print_array
}