
如何通过编码实现 openssl 命令的某项功能。

以打印模式值为例 `openssl rsa -modules`。

首先打开 openssl 源码，在搜索框中查找 `modules` 字样，可能会出现很多。

我们可以通过执行 `openssl rsa --help` 来找其他的:
```s
Usage: rsa [options]
Valid options are:
 -help              Display this summary
 -inform format     Input format, one of DER PEM
 -outform format    Output format, one of DER PEM PVK
 -in val            Input file
 -out outfile       Output file
 -pubin             Expect a public key in input file
 -pubout            Output a public key
 -passout val       Output file pass phrase source
 -passin val        Input file pass phrase source
 -RSAPublicKey_in   Input is an RSAPublicKey
 -RSAPublicKey_out  Output is an RSAPublicKey
 -noout             Don't print key out
 -text              Print the key in text
 -modulus           Print the RSA key modulus
 -check             Verify key consistency
 -*                 Any supported cipher
 -pvk-strong        Enable 'Strong' PVK encoding level (default)
 -pvk-weak          Enable 'Weak' PVK encoding level
 -pvk-none          Don't enforce PVK encoding
 -engine val        Use engine, possibly a hardware device
```
如上面所示，查找 `RSAPublicKey_in`，可以看到只有很少几个。

在 apps/rsa.c 文件中找到了真正的参数实现，如下:
```c++
  const OPTIONS rsa_options[] = {
    {"help", OPT_HELP, '-', "Display this summary"},
    {"inform", OPT_INFORM, 'f', "Input format, one of DER PEM"},
    {"outform", OPT_OUTFORM, 'f', "Output format, one of DER PEM PVK"},
    {"in", OPT_IN, 's', "Input file"},
    {"out", OPT_OUT, '>', "Output file"},
    {"pubin", OPT_PUBIN, '-', "Expect a public key in input file"},
    {"pubout", OPT_PUBOUT, '-', "Output a public key"},
    {"passout", OPT_PASSOUT, 's', "Output file pass phrase source"},
    {"passin", OPT_PASSIN, 's', "Input file pass phrase source"},
    {"RSAPublicKey_in", OPT_RSAPUBKEY_IN, '-', "Input is an RSAPublicKey"},
    {"RSAPublicKey_out", OPT_RSAPUBKEY_OUT, '-', "Output is an RSAPublicKey"},
    {"noout", OPT_NOOUT, '-', "Don't print key out"},
    {"text", OPT_TEXT, '-', "Print the key in text"},
    {"modulus", OPT_MODULUS, '-', "Print the RSA key modulus"},
    {"check", OPT_CHECK, '-', "Verify key consistency"},
    {"", OPT_CIPHER, '-', "Any supported cipher"},
  #if !defined(OPENSSL_NO_DSA) && !defined(OPENSSL_NO_RC4)
    {"pvk-strong", OPT_PVK_STRONG, '-', "Enable 'Strong' PVK encoding level (default)"},
    {"pvk-weak", OPT_PVK_WEAK, '-', "Enable 'Weak' PVK encoding level"},
    {"pvk-none", OPT_PVK_NONE, '-', "Don't enforce PVK encoding"},
  #endif
  #ifndef OPENSSL_NO_ENGINE
    {"engine", OPT_ENGINE, 's', "Use engine, possibly a hardware device"},
  #endif
    {NULL}
  };
```

然后在这个文件中搜索 `modules`，其实现如下:
```c++
  if (modulus) {
    const BIGNUM *n;
    RSA_get0_key(rsa, &n, NULL, NULL);
    BIO_printf(out, "Modulus=");
    BN_print(out, n);
    BIO_printf(out, "\n");
  }
```
