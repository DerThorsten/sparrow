local ffi = require("ffi")

ffi.cdef[[
  void free(void *ptr);
  void *malloc(size_t size);
  
]]
