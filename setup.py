import setuptools
from torch.utils import cpp_extension


setuptools.setup(
    name='th-seal',
    ext_modules=[
        cpp_extension.CppExtension(
            'th_seal',
            ['th_seal.cpp'],
            extra_compile_args=['-std=c++17'],
            extra_link_args=['-lseal-3.4']
            ),
        ],
    cmdclass={'build_ext': cpp_extension.BuildExtension}
)
