<div align="right" style="margin-bottom: 20px;">
  🌐 <strong>Languages:</strong>
  <a href="https://gitee.com/sz-ai/nlurh/blob/master/docs/dll/crypt_zh.md" 
     target="_blank" 
     style="margin-left: 10px;">
    🇨🇳 中文 (Gitee)
  </a>
</div>
------

# Data Encryption Component Library crypt.dll Usage Guide

Z-Language Interface Specification

------

### 1: Basic Interfaces(ArrayName"tls": using the out tls command can display all subset interfaces))

```jade
---<out tls
[tls].sub(getasn x509cw x509w x509v gp10 pk_free pk_rsapri pk_rsapub pk_verify pk_sign pk_dec pk_enc pk_outpri pk_outpub pk_inkey pk_get cbcd cbce ecbd ecbe mdc md b64d b64e stb bts rand )
```

------



## 1. Data Processing

##### 1.1: Hex to String

| Interface | `tls.bts` | Converts hex to ASCII string (doubles length) |
| --------- | --------- | --------------------------------------------- |
| Parameter | `buf`     | Binary data buffer                            |
| Returns   | `str`     | ASCII string (double original length)         |

```
str tls.bts(buf)  
/* Example:  
tls.bts("\xA1\xB2\xC3") returns "A1B2C3"  
tls.bts'A1B2c3' returns "A1B2C3"  
*/  
```

##### 1.2: String to Hex

| Interface | `tls.stb` | Converts ASCII string to hex (halves length) |
| --------- | --------- | -------------------------------------------- |
| Parameter | `str`     | ASCII string (valid chars: 0-9, A-F, a-f)    |
| Returns   | `buf`     | Binary data buffer                           |

```
buf tls.stb(str)  
/* Example:  
tls.stb("A1B2C3") returns '\xA1\xB2\xC3'  
tls.stb"303132" returns '\x30\x31\x32' (equals "012")  
*/  
```

##### 1.3: Base64 Encode

| Interface | `tls.b64e` | Base64 encoding (4/3 output length) |
| --------- | ---------- | ----------------------------------- |
| Parameter | `buf`      | Binary data buffer                  |
| Returns   | `str`      | Base64-encoded string               |

```
str tls.b64e(buf)  
/* Example:  
tls.b64e('\x11\x22\x33') returns "ESIz"  
*/  
```

##### 1.4: Base64 Decode

| Interface | `tls.b64d` | Base64 decoding (3/4 output length) |
| --------- | ---------- | ----------------------------------- |
| Parameter | `str`      | Base64-encoded string               |
| Returns   | `buf`      | Binary data buffer                  |

```
buf tls.b64d(str)  
/* Example:  
tls.b64d("ESIz") returns '\x11\x22\x33'  
*/  
```

##### 1.5: Random Number Generator

| Interface | `tls.rand` | Generates cryptographically secure random data |
| --------- | ---------- | ---------------------------------------------- |
| Parameter | `len`      | Output length (optional)                       |
| Returns   | Mixed      | Random number (no param) or random buffer      |

```
mixed tls.rand(len)  
/* Example:  
tls.rand() returns integer: -266015095  
tls.rand(16) returns 16-byte buffer  
*/  
```

------

## 2. Digest Algorithms

##### 2.1: Hash Algorithm

| Interface  | `tls.md`       | Computes message digest                                      |
| ---------- | -------------- | ------------------------------------------------------------ |
| Parameters | `flag`  `data` | Algorithm ID ("md5", "sha1", "sha256", "sm3")  Input data (buffer/string) |
| Returns    | `buf`          | Digest value (false on error)                                |

```
buf tls.md(flag, data)  
/* Example:  
tls.md("md5", "abcdef123456789") returns 16-byte digest  
*/  
```

##### 2.2: HMAC Algorithm

| Interface  | `tls.mdc`             | Computes keyed-hash MAC              |
| ---------- | --------------------- | ------------------------------------ |
| Parameters | `flag`  `key`  `data` | Algorithm ID  Secret key  Input data |
| Returns    | `buf`                 | MAC value (false on error)           |

```
buf tls.mdc(flag, key, data)  
```

------

## 3. Symmetric Encryption

##### 3.1: ECB Encryption

| Interface  | `tls.ecbe`            | ECB mode encryption                                   |
| ---------- | --------------------- | ----------------------------------------------------- |
| Parameters | `flag`  `key`  `data` | Algorithm ("des","aes","sm4")  Secret key  Input data |
| Returns    | `buf`                 | Ciphertext (false on error)                           |

```
buf tls.ecbe(flag, key, data)  
```

##### 3.2: ECB Decryption

| Interface  | `tls.ecbd`            | ECB mode decryption                  |
| ---------- | --------------------- | ------------------------------------ |
| Parameters | `flag`  `key`  `data` | Algorithm ID  Secret key  Ciphertext |
| Returns    | `buf`                 | Plaintext (false on error)           |

```
buf tls.ecbd(flag, key, data)  
```

##### 3.3: CBC Encryption

| Interface  | `tls.cbce`                  | CBC mode encryption                                      |
| ---------- | --------------------------- | -------------------------------------------------------- |
| Parameters | `flag`  `key`  `data`  `iv` | Algorithm ID  Secret key  Input data  IV (default: NULL) |
| Returns    | `buf`                       | Ciphertext (false on error)                              |

```
buf tls.cbce(flag, key, data, iv)  
```

##### 3.4: CBC Decryption

| Interface  | `tls.cbcd`                  | CBC mode decryption                                      |
| ---------- | --------------------------- | -------------------------------------------------------- |
| Parameters | `flag`  `key`  `data`  `iv` | Algorithm ID  Secret key  Ciphertext  IV (default: NULL) |
| Returns    | `buf`                       | Plaintext (false on error)                               |

```
buf tls.cbcd(flag, key, data, iv)  
```

------

## 4. Asymmetric Cryptography

##### 4.1: Generate Key Pair

| Interface  | `tls.pk_get`  | Generates key pair                                           |
| ---------- | ------------- | ------------------------------------------------------------ |
| Parameters | `key`  `flag` | Output format (""=PEM, "bin"=raw)  Algorithm ("sm2","rsa:1024","rsa:2048") |
| Returns    | `ctx`         | Key context (false on error)                                 |

```
ctx tls.pk_get(key, flag)  
```

##### 4.2: Import Keys

| Interface | `tls.pk_inkey` | Imports keys from PEM/CER           |
| --------- | -------------- | ----------------------------------- |
| Parameter | `key`          | PEM/CER formatted key               |
| Returns   | `ctx`          | Key context (error code on failure) |

```
ctx tls.pk_inkey(key)  
```

##### 4.3: Release Key Context

| Interface | `tls.pk_free` | Releases key resources |
| --------- | ------------- | ---------------------- |
| Parameter | `ctx`         | Key context            |
| Returns   | `bool`        | Success status         |

```
bool tls.pk_free(ctx)  
```

##### 4.4: Export Public Key

| Interface | `tls.pk_outpub` | Exports public key                   |
| --------- | --------------- | ------------------------------------ |
| Parameter | `ctx`           | Key context                          |
| Returns   | `buf`           | ASN.1 formatted key (false on error) |

```
buf tls.pk_outpub(ctx)  
```

##### 4.5: Export Private Key

| Interface | `tls.pk_outpri` | Exports private key                        |
| --------- | --------------- | ------------------------------------------ |
| Parameter | `ctx`           | Key context                                |
| Returns   | `buf`           | ASN.1 formatted key (false if unavailable) |

```
buf tls.pk_outpri(ctx)  
```

##### 4.6: Public Key Encryption

| Interface  | `tls.pk_enc`             | Encrypts with public key                      |
| ---------- | ------------------------ | --------------------------------------------- |
| Parameters | `key`  `data`  `padding` | Key context  Plaintext  Padding ("v15","v21") |
| Returns    | `buf`                    | Ciphertext (error code on failure)            |

```
buf tls.pk_enc(key, data, padding)  
```

##### 4.7: Private Key Decryption

| Interface  | `tls.pk_dec`             | Decrypts with private key         |
| ---------- | ------------------------ | --------------------------------- |
| Parameters | `key`  `data`  `padding` | Key context  Ciphertext  Padding  |
| Returns    | `buf`                    | Plaintext (error code on failure) |

```
buf tls.pk_dec(key, data, padding)  
```

##### 4.8: Private Key Signing

| Interface  | `tls.pk_sign`           | Creates digital signature                                 |
| ---------- | ----------------------- | --------------------------------------------------------- |
| Parameters | `key`  `data`  `scheme` | Key context  Data to sign  Scheme ("sha1v15","sha256v21") |
| Returns    | `buf`                   | Signature (error code on failure)                         |

```
buf tls.pk_sign(key, data, scheme)  
```

##### 4.9: Public Key Verification

| Interface  | `tls.pk_verify`                 | Verifies digital signature                    |
| ---------- | ------------------------------- | --------------------------------------------- |
| Parameters | `key`  `data`  `sign`  `scheme` | Key context  Original data  Signature  Scheme |
| Returns    | `bool`                          | Verification result                           |

```
bool tls.pk_verify(key, data, sign, scheme)  
```

##### 4.10: RSA Public Key Operation

| Interface  | `tls.pk_rsapub` | Raw RSA public operation       |
| ---------- | --------------- | ------------------------------ |
| Parameters | `key`  `data`   | Key context  Input data        |
| Returns    | `buf`           | Result (false on length error) |

```
buf tls.pk_rsapub(key, data)  
```

##### 4.11: RSA Private Key Operation

| Interface  | `tls.pk_rsapri` | Raw RSA private operation      |
| ---------- | --------------- | ------------------------------ |
| Parameters | `key`  `data`   | Key context  Input data        |
| Returns    | `buf`           | Result (false on length error) |

```
buf tls.pk_rsapri(key, data)  
```

------

## 5. Certificates

##### 5.1: Generate CSR

| Interface  | `tls.gp10`                   | Creates PKCS#10 certificate request                       |
| ---------- | ---------------------------- | --------------------------------------------------------- |
| Parameters | `key`  `subname`  `strength` | Key context  Subject DN  Hash strength (optional for SM2) |
| Returns    | `str`                        | PEM-formatted CSR (false on error)                        |

```
str tls.gp10(key, subname, strength)  
```

##### 5.2: Certificate Verification

| Interface  | `tls.x509v`      | Verifies certificate chain      |
| ---------- | ---------------- | ------------------------------- |
| Parameters | `cer`  `cerlast` | Certificate  Issuer certificate |
| Returns    | `bool`           | Verification result             |

```
bool tls.x509v(cer, cerlast)  
```

##### 5.3: Issue Certificate

| Interface  | `tls.x509cw`                          | Issues X.509 certificate                                     |
| ---------- | ------------------------------------- | ------------------------------------------------------------ |
| Parameters | `sn`  `md`  `p10`  `isskey`  `rootca` | Serial number  Algorithm ID  CSR  Issuer private key  Root CA |
| Returns    | `str`                                 | PEM certificate (false on error)                             |

```
str tls.x509cw(sn, md, p10, isskey, rootca)  
```

**Implementation Guide**:
 [Natural Language Understanding NLU - Third-party Integration](https://gitee.com/kebo521/nlu3)

