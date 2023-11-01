
### openssl 中的证书验证

openssl 中的证书验证比较复杂,实现源码在 x509/x509_vfy.c 中,主要有两个函数: `X509_verify_cert` 和 `internal_verify`。

X509_verify_cert 主要将所有的证书信息进行排序,构造出一个有序的证书链,然后调用 internal_verify 函数来验证证书。

internal_verify 是 openssl 提供的一个内置的验证证书链的函数。如果用户通过 X509_STORE_set_verify_func 函数设置了 X509_STORE_CTX 的 verify 函数,将调用用户实现的 verify 函数而不会调用 internal_verify。

如何用 openssl 函数验证证书,用户可以参考 apps/verify.c。
