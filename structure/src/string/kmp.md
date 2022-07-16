# KMP

获取next数组的逻辑时序示例

```shell
# ---- next[0] = -1
# i = 0, j = -1

# ---- next[1]
#   i = 0 
#   a b a a b c a c
#   a b a a b c a c
# j = -1
#
#   i = 0 
#   a b a a b c a c
#   a b a a b c a c
# j = -1 
# next[1] = j = -1
# j == -1 => ij++
#
#     i = 1 
#   a b a a b c a c
#     a b a a b c a c
#     j = 0
# next[i(=1)] = j = 0 
# t[a] != t[b] => j = next[j] = -1 => ij++ & next[i(=2)] = j = 0

# ---- next[2]
#       i = 2 
#   a b a a b c a c
#       a b a a b c a c
#       j = 0
```