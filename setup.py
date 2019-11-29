import setuptools
# from torch.utils import cpp_extension


setuptools.setup(
    name='th-seal',
    ext_modules=[
        setuptools.Extension(
            name='th_seal',
            sources=['th_seal.cpp'],
            # include_dirs=cpp_extension.include_paths(),
            language='c++',
            extra_compile_args=['-std=c++17'],
            extra_link_args=['-lseal-3.4', '-lz']
            ),
        ],
    # cmdclass={'build_ext': cpp_extension.BuildExtension}
)
