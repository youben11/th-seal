# Torch SEAL

### Note

This branch only uses pybind11 to bind SEAL functionalities to Python. It's not built around PyTorch.


This repo is a PoC for building a PyTorch extension around [Microsoft SEAL library](https://github.com/Microsoft/SEAL).

I currently have installed and setuped SEAL manually on my machine. This PyTorch extension is only binding some random SEAL code as th_seal.poc_seal() to check if the compilation, link and import are working correctly.

You can test the PoC by first installing th_seal

```bash
$ # You first need to install SEAL on your machine
$ git clone https://github.com/youben11/th-seal
$ cd th-seal
$ pip setup.py install
```

then test if everything is working correctly using this code snippet

```python
import torch
import th_seal

th_seal.poc_seal(torch.tensor(73))
```
