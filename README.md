# smoothd
Smooth out IO operations on a potentially spinning disk.

The purpose of this dameon is to perform file delete and file
copy operations in such a way that they are coordinated and
do not thrash the target disk.

