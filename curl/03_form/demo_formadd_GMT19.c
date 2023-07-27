
typedef struct {
	// ... 句柄里的东西
	
	char* url;
	char* passwd;
	int urllen;
	int passwdlen;
} AppHandle;

int SAF_Pkcs7_EncodeData(
	void * hAppHandle,						// 全局句柄
	unsigned char * pucSignContainerName,	// used 密钥id
	unsigned int uiSignContainerNameLen,	// used
	unsigned int uiSignKeyUsage,			// unprocessed 私钥用途
	unsigned char * pucSignerCertificate,	// unprocessed 签发者证书
	unsigned int uiSignerCertificateLen,	// unprocessed
	
	unsigned int uiDigestAlgorithms,		// used 哈希算法
	unsigned char * pucEncCertificate,		// unprocessed 接收者证书
	unsigned int uiEncCertificateLen,		// unprocessed
	unsigned int uiSymmAlgorithm,			// unprocessed 对称算法参数
	
	unsigned char * pucData,				// used 原始数据
	unsigned int uiDataLen,					// used
	unsigned char * pucDerP7Data,			
	unsigned int * puiDerP7DataLen)
{
	CURL* curl;
	CURLcode res;
	struct curl_httppost* formpost = NULL;
	struct curl_httppost* lastptr = NULL;
	
	handle = (AppHandle*)hAppHandle;
	
	// 口令
	curl_formadd(
				CURLFORM_COPYNAME,			"passwd",
				CURLFORM_PTRCONTENTS,		handle->passwd,
				CURLFORM_CONTENTSLENGTH,	handle->passwdlen,
				CURLFORM_END
				);
	// 签名私钥的容器名，也即密钥的 id
	curl_formadd(
				CURLFORM_COPYNAME,			"id";
				CURLFORM_PTRCONTENTS,		pucSignContainerName,
				CURLFORM_CONTENTSLENGTH,	uiSignContainerNameLen,
				CURLFORM_END
				);
	// 不处理 uiSignKeyUsage
	// 不处理 pucSignerCertificate
	// 不处理 pucEncCertificate
	// 不处理 uiSymmAlgorithm
	// 原始数据
	curl_formadd(
				CURLFORM_COPYNAME,			"data",
				CURLFORM_PTRCONTENTS,		pucData,
				CURLFORM_CONTENTSLENGTH,	uiDataLen,
				CURLFORM_END
				);
	// 哈希算法
	curl_formadd(
				CURLFORM_COPYNAME,			'algo',
				CURLFORM_COPYCONTENTS,		itoa(uiDigestAlgorithms),
				CURLFORM_END
				);
	// 提交
	curl_formadd(
				CURLFORM_COPYNAME,			'sumbit',
				CURLFORM_COPYCONTENTS,		'send',
				CURLFORM_END
				);
		
	curl = curl_easy_init();
	curl_global_init(CURL_GLOBAL_ALL);
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, handle->url);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writecb);
		
		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_form() failed: %s\n", curl_easy_strerror(res));
			goto cleanup;
		}
	}
	
cleanup:
	curl_easy_cleanup(curl);
	curl_formfree(formpost);
	
	curl_global_cleanup()
	
	return res;
}