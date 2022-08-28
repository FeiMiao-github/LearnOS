import os

PREFIX = 'riscv64-unknown-elf-'
CXX = PREFIX + 'g++'
CXXFLAGS = '-nostdlib -fno-builtin -mcmodel=medany -mabi=lp64d -std=c++2a -g'
CC = PREFIX + 'gcc'
CCFLAGS = '-nostdlib -fno-builtin -fno-omit-frame-pointer -mcmodel=medany -mabi=lp64d -g -Wall -O2'
LD = PREFIX + 'ld'
LINKFLAGS = '-nostdlib -fno-builtin -mcmodel=medany -mabi=lp64d '
AR = PREFIX + 'ar'
OBJCOPY = PREFIX + 'objcopy'
OBJDUMP = PREFIX + 'objdump'
READELF = PREFIX + 'readelf'

def ElfToBin(source, target, env, for_signature):
    return '{objcopy} {src} --strip-all -O binary {tgt}'.format(objcopy=env['OBJCOPY'], src=str(source[0]), tgt=str(target[0]))

def DAsm(source, target, env, for_signature):
    return '{objdump} -S {src} > {target}'.format(objdump=env['OBJDUMP'],src=source[0],target=target[0])

def GenSym(source, target, env, for_signature):
    return '{readelf} -a {src} > {target}'.format(readelf=env['READELF'],src=source[0],target=target[0])

env = Environment(ENV=os.environ.copy())
env['CXX'] = CXX
env['CXXFLAGS'] = CXXFLAGS
env['CC'] = CC
env['CCFLAGS'] = CCFLAGS
env['LD'] = LD
env['LINKFLAGS'] = LINKFLAGS
env['AR'] = AR
env['OBJCOPY'] = OBJCOPY
env['OBJDUMP'] = OBJDUMP
env['READELF'] = READELF

env['BUILDERS']['ElfToBin'] = Builder(generator=ElfToBin, suffix='.bin', src_suffix='.elf')
env['BUILDERS']['DAsm'] = Builder(generator=DAsm, suffix='.elf', src_suffix='.asm')
env['BUILDERS']['GenSym'] = Builder(generator=GenSym, suffix='.elf', src_suffix='.sym')

env.Tool('compilation_db')
env.CompilationDatabase()  

userBins =  env.SConscript(dirs='user', variant_dir='build/user', duplicate=0, exports ='env')

os_env = env.Clone()
linkScript = 'os/linker.ld'
os_env['LINKFLAGS'] += ' -T {}'.format(linkScript)
os_env['CPPDEFINES'] = {
    'PAGE_TEST': None,
    'TRAP_TEST': None,
    'PREEMPTIVE_TEST': None,
    'USE_LCK': None
}

os_objects = os_env.SConscript(dirs='os', variant_dir='build/os', duplicate=0, exports ='os_env')
os_env.Depends(target=os_objects, dependency=userBins)

os_env.Program('#bin/hello.elf', os_objects)
os_env.ElfToBin(source='#bin/hello.elf',target='#bin/hello.bin')
os_env.DAsm(source='#bin/hello.elf',target='#bin/hello.asm')
os_env.GenSym(source='#bin/hello.elf',target='#bin/hello.sym')