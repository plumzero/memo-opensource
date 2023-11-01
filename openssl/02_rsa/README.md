
参考:
- [加密算法之PKCS填充](https://blog.csdn.net/qq_27575841/article/details/123861528)

### 公私钥文件

RSA 加密的 public key 格式有多种，常见的有两种。

一种密钥头为`-----BEGIN RSA PUBLIC KEY-----`,一种开头为`-----BEGIN PUBLIC KEY-----`，二者分别对应 RSA 的 PKCS#1 和 PKCS#8 格式。使用 OpenSSL 库加载 RSA 的公钥时，使用的函数也不同。

以字符串公钥为例，对 PKCS#1 格式的密钥加载使用 `PEM_read_bio_RSAPublicKey()` 函数，对 PKCS#8 格式公钥的加载使用 `PEM_read_bio_RSA_PUBKEY()` 函数。私钥读取通常使用 `PEM_read_bio_RSAPrivateKey()` 函数。

### PKCS1 填充

采用 RSA_PKCS1_PADDING 填充时，无论公钥加密还是私钥加密，输入必须 比 RSA 秘钥短至少 11 个字节, 也就是 RSA_size(rsa) - 11，输出则和秘钥一样长。

### RSA 签名验签

RSA_sign 函数将需要运算的数据放入 X509_ALGOR 数据结构并将其 DER 编码,对编码结果做 RSA_PKCS1_PADDING 再进行私钥加密。被签名数据应该是摘要之后的数据。

RSA_verify 用来验证签名。
