#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <seal/seal.h>
#include <iostream>

using namespace seal;
using namespace seal::util;
using namespace std;
namespace py = pybind11;

EncryptionParameters get_parms() {
    EncryptionParameters parms(scheme_type::CKKS);
    size_t poly_modulus_degree = 8192;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(
        CoeffModulus::Create(poly_modulus_degree, {60, 40, 40, 60}));

    return parms;
}

std::shared_ptr<seal::SEALContext> get_context(EncryptionParameters parms) {
    auto context = SEALContext::Create(parms);
    return context;
}

Ciphertext encrypt(std::shared_ptr<seal::SEALContext> context,
                   PublicKey pub_key, vector<double> pt) {
    static double scale = pow(2.0, 40);

    Encryptor encryptor(context, pub_key);
    CKKSEncoder encoder(context);
    Plaintext plain_text;
    Ciphertext ciphertext;

    encoder.encode(pt, scale, plain_text);
    encryptor.encrypt(plain_text, ciphertext);

    return ciphertext;
}

vector<double> decrypt(std::shared_ptr<seal::SEALContext> context,
                       SecretKey secret_key, Ciphertext ct) {
    Decryptor decryptor(context, secret_key);
    CKKSEncoder encoder(context);
    Plaintext plain_text;

    decryptor.decrypt(ct, plain_text);
    vector<double> pt;
    encoder.decode(plain_text, pt);

    return pt;
}

PYBIND11_MODULE(th_seal, m) {
    m.def("get_parms", &get_parms, "get parameters");
    m.def("get_context", &get_context, "get context");
    m.def("encrypt", &encrypt, "encrypt double");
    m.def("decrypt", &decrypt, "encrypt double");

    py::class_<KeyGenerator>(m, "KeyGenerator")
        .def(py::init<std::shared_ptr<seal::SEALContext>&>())
        .def("public_key", &KeyGenerator::public_key)
        .def("secret_key", &KeyGenerator::secret_key);

    py::class_<PublicKey>(m, "PublicKey");
    py::class_<SecretKey>(m, "SecretKey");

    py::class_<Plaintext>(m, "Plaintext");
    py::class_<Ciphertext>(m, "Ciphertext");

    py::class_<SEALContext, std::shared_ptr<SEALContext>>(m, "SEALContext")
        .def("Create", &SEALContext::Create, "Create new context",
             py::arg("expand_mod_chain") = true, py::arg("sec_level") = 128);

    py::class_<EncryptionParameters>(m, "EncryptionParameters")
        .def("set_poly_modulus_degree",
             &EncryptionParameters::set_poly_modulus_degree);

    py::class_<Encryptor>(m, "Encryptor")
        .def(py::init<std::shared_ptr<seal::SEALContext>&, seal::PublicKey>())
        .def("encrypt", &Encryptor::encrypt);

    py::class_<Decryptor>(m, "Decryptor")
        .def(py::init<std::shared_ptr<seal::SEALContext>&, seal::SecretKey>())
        .def("decrypt", &Decryptor::decrypt);
}