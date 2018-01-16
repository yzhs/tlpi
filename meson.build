project('tlpi', 'c', default_options:
    ['c_std=c11', 'warning_level=3', 'werror=true'])

# Chapter 5
executable('tee', 'tee/tee.c')
executable('copy-content', 'cp/copy_file_content.c')

# Chapter 6
executable('large_file', 'fileio/large_file.c')
executable('append_seek', 'fileio/append_seek.c')
executable('atomic_append', 'fileio/atomic_append.c')
executable('dup', 'fileio/dup.c')
executable('dup_fd_offset', 'fileio/dup_fd_offset.c')

# Chapter 7
executable('env', 'processes/env.c')

# Chapter 8
#executable('malloc', 'memory/malloc.c') # TODO implement malloc and free

# Chapter 9
executable('getpwnam', 'users/getpwnam.c')

# Chapter 10
executable('uid', 'processes/uid.c')
