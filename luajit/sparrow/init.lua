local sparrow = {}

-- -- Require submodules
local buffer = require("sparrow.buffer")
sparrow.Buffer = buffer.Buffer

local bitset = require("sparrow.bitset")
sparrow.Bitset = bitset.Bitset


sparrow.layouts = require("sparrow.layouts")


return sparrow