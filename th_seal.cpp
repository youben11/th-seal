#include <iostream>
#include <seal/ciphertext.h>
#include <seal/context.h>
#include <seal/keygenerator.h>
#include <seal/encryptor.h>
#include <seal/memorymanager.h>
#include <seal/modulus.h>
#include <pybind11/pybind11.h>

using namespace seal;
using namespace seal::util;
using namespace std;

void test() {
  // Some random code from SEAL's test
  // just to make sure the extension should work with SEAL

  stringstream stream;
  EncryptionParameters parms(scheme_type::BFV);
  parms.set_poly_modulus_degree(2);
  parms.set_coeff_modulus(CoeffModulus::Create(2, { 30 }));
  parms.set_plain_modulus(2);

  auto context = SEALContext::Create(parms, false, sec_level_type::none);

  Ciphertext ctxt(context);
  Ciphertext ctxt2;
  ctxt.save(stream);
  ctxt2.load(context, stream);

  parms.set_poly_modulus_degree(1024);
  parms.set_coeff_modulus(CoeffModulus::BFVDefault(1024));
  parms.set_plain_modulus(0xF0F0);
  context = SEALContext::Create(parms, false);
  KeyGenerator keygen(context);
  Encryptor encryptor(context, keygen.public_key());
  encryptor.encrypt(Plaintext("Ax^10 + 9x^9 + 8x^8 + 7x^7 + 6x^6 + 5x^5 + 4x^4 + 3x^3 + 2x^2 + 1"), ctxt);
  ctxt.save(stream);
  ctxt2.load(context, stream);
}

PYBIND11_MODULE(th_seal, m) {
  m.def("poc_seal", &test, "seal func4");
}