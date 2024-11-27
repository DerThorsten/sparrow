-- import larrow
sp = require("sparrow")


local size = 5

-- data
local data_buffer = sp.Buffer.new(size, "float")
for i = 0, data_buffer.size - 1 do
    data_buffer[i] = i
end


-- bitmap
local validity = sp.Bitset.new(size)
validity[0] = true
validity[1] = true
validity[2] = false
validity[3] = true
validity[4] = false


local arr = sp.layouts.NumericLayout:new(data_buffer, validity)

arr:print()

print("sub_arr")
sub_arr = arr:_subrange(1, 3)
for i = 0, sub_arr.size - 1 do
    print(sub_arr:get(i))
end



local arr_no_null = sp.layouts.NumericLayout:new(data_buffer)

arr_no_null:print()

print("sub_arr arr_no_null")
sub_arr = arr_no_null:_subrange(1, 3)
for i = 0, sub_arr.size - 1 do
    print(sub_arr:get(i))
end