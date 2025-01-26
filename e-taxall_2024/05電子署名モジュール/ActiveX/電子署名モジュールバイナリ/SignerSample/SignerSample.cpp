// SignerSample.cpp : アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#import "..\\Bin\\CLXtxSigner.dll"

void AddSignatureToReport();
void AddSignatureToCertRegist();
void VerifySignature();
void AddSignatureToReportICCard();
void AddSignatureToCertRegistICCard();
void VerifySignature();

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
	// COMライブラリ初期化
	CoInitialize(NULL);

	try {

	// 電子証明書ファイルを利用するケース

		// 申告等データファイルへの署名付与（電子証明書ファイル)
		AddSignatureToReport();

		// 電子証明書更新申請への署名付与（電子証明書ファイル)
		AddSignatureToCertRegist();

	// ICカードを利用するケース

		// 申告等データファイルへの署名付与（ICカード)
		AddSignatureToReportICCard();

		// 電子証明書更新申請への署名付与（ICカード)
		AddSignatureToCertRegistICCard();

	// 受信通知の署名検証（共通)
		VerifySignature();


	// COM例外ハンドラ
	} catch(_com_error e) {
		MessageBox(0, e.Description(), e.ErrorMessage(), MB_OK);
		CoUninitialize();
		return e.Error();
	}

	// COMライブラリ終期化
	CoUninitialize();

	return 0;
}

void AddSignatureToReport()
{
	// ネームスペース宣言
	using namespace CLXTXSIGNERLib;

	try {

		// 署名モジュールインスタンス生成
		CLISignaturePtr pISignature;
		HRESULT hr = pISignature.CreateInstance(__uuidof(CLCXtxSigner));
		if (FAILED(hr)) {
			_com_raise_error(hr);
		}

		// PKCS#12ファイル設定
		_bstr_t bstrPKCS12("..\\Cert\\CliCert000018.p12");
		_bstr_t bstrPassword("41wh602z");
		CLICertificateInfoPtr pICertInfo = pISignature->SetCertificate(bstrPKCS12, bstrPassword);

		// 有効期間チェック
		_variant_t varValidity = pICertInfo->IsValid();
		if (varValidity.boolVal == VARIANT_FALSE) {
			MessageBox(0, "有効期間切れ", "署名モジュール", MB_OK);
			_com_raise_error(E_INVALIDARG);
		}

		// 証明書情報取得
		_bstr_t bstrIssuer(pICertInfo->GetIssuer(), false);
		_bstr_t bstrSubject(pICertInfo->GetSubject(), false);

		// 申告等データファイルへの署名付与
		_bstr_t bstrXML       = "..\\Data\\RHO0010.xtx";
		_bstr_t bstrSignedXML = "..\\Data\\RHO0010signed.xtx";
		hr = pISignature->SignToReport(bstrXML, bstrSignedXML, "", "1777187306200700");
		if (FAILED(hr)) {
			_com_raise_error(hr);
		}

	// COM例外ハンドラ
	} catch(_com_error e) {
		MessageBox(0, e.Description(), e.ErrorMessage(), MB_OK);
		throw;
	}
}

void AddSignatureToCertRegist()
{
	// ネームスペース宣言
	using namespace CLXTXSIGNERLib;

	try {

		// 署名モジュールインスタンス生成
		CLISignaturePtr pISignature;
		HRESULT hr = pISignature.CreateInstance(__uuidof(CLCXtxSigner));
		if (FAILED(hr)) {
			_com_raise_error(hr);
		}

		// PKCS#12ファイル設定（新電子証明書）
		_bstr_t bstrPKCS12("..\\Cert\\CliCert000018.p12");
		_bstr_t bstrPassword("41wh602z");
		CLICertificateInfoPtr pICertInfo = pISignature->SetCertificate(bstrPKCS12, bstrPassword);

		// 有効期間チェック
		_variant_t varValidity = pICertInfo->IsValid();
		if (varValidity.boolVal == VARIANT_FALSE) {
			MessageBox(0, "有効期間切れ", "署名モジュール", MB_OK);
			_com_raise_error(E_INVALIDARG);
		}

		// 電子証明書更新申請への新電子証明書での署名付与
		_bstr_t bstrXML       = "..\\Data\\PTE0010.xtx";
		_bstr_t bstrSignedXML = "..\\Data\\PTE0010signed.xtx";
		hr = pISignature->SignToCertificateRegistration(bstrXML, bstrSignedXML, 1, "", "1777187306200700");
		if (FAILED(hr)) {
			_com_raise_error(hr);
		}
		
		// 署名モジュールインスタンス破棄
		pISignature = NULL;

		// 署名モジュールインスタンス再生成
		hr = pISignature.CreateInstance(__uuidof(CLCXtxSigner));
		if (FAILED(hr)) {
			_com_raise_error(hr);
		}
		// PKCS#12ファイル設定（旧電子証明書）
		bstrPKCS12   = "..\\Cert\\CliCert000019.p12";
		bstrPassword = "hj24f2et";
		pICertInfo = pISignature->SetCertificate(bstrPKCS12, bstrPassword);

		// 有効期間チェック
		varValidity = pICertInfo->IsValid();
		if (varValidity.boolVal == VARIANT_FALSE) {
			MessageBox(0, "有効期間切れ", "署名モジュール", MB_OK);
			_com_raise_error(E_INVALIDARG);
		}

		// 電子証明書更新申請への旧電子証明書での署名付与
		bstrXML       = "..\\Data\\PTE0010signed.xtx";
		bstrSignedXML = "..\\Data\\PTE0010signed.xtx";
		hr = pISignature->SignToCertificateRegistration(bstrXML, bstrSignedXML, 2, "", "1777187306200700");
		if (FAILED(hr)) {
			_com_raise_error(hr);
		}

	// COM例外ハンドラ
	} catch(_com_error e) {
		MessageBox(0, e.Description(), e.ErrorMessage(), MB_OK);
		throw;
	}
}

void VerifySignature()
{
	// ネームスペース宣言
	using namespace CLXTXSIGNERLib;

	try {

		// 署名モジュールインスタンス生成
		CLISignaturePtr pISignature;
		HRESULT hr = pISignature.CreateInstance(__uuidof(CLCXtxSigner));
		if (FAILED(hr)) {
			_com_raise_error(hr);
		}

		// 受信通知の署名検証
		_bstr_t bstrXML = "..\\Data\\MailDtilGmnViewCtl.xml";
		_variant_t varStatus = pISignature->VerifyResponse(bstrXML);
		if (varStatus.vt == VT_I4) {
			if (varStatus.lVal != 0) {
				varStatus.ChangeType(VT_BSTR);
				MessageBox(0, (char*)varStatus.bstrVal, "検証エラー", MB_OK);
			}
		}

	// COM例外ハンドラ
	} catch(_com_error e) {
		MessageBox(0, e.Description(), e.ErrorMessage(), MB_OK);
		throw;
	}
}

void AddSignatureToReportICCard()
{
	// ネームスペース宣言
	using namespace CLXTXSIGNERLib;

	try {

		// 署名モジュールインスタンス生成
		CLISignaturePtr pISignature;
		HRESULT hr = pISignature.CreateInstance(__uuidof(CLCXtxSigner));
		if (FAILED(hr)) {
			_com_raise_error(hr);
		}

		// ICカード接続
		_bstr_t bstrCSPName("JPKI Crypto Service Provider");// 公的個人認証サービスのCSPを利用する場合
		CLICertificateInfoPtr pICertInfo = pISignature->SetCertificateICCard(bstrCSPName);

		// 有効期間チェック
		_variant_t varValidity = pICertInfo->IsValid();
		if (varValidity.boolVal == VARIANT_FALSE) {
			MessageBox(0, "有効期間切れ", "署名モジュール", MB_OK);
			_com_raise_error(E_INVALIDARG);
		}

		// 証明書情報取得
		_bstr_t bstrIssuer(pICertInfo->GetIssuer(), false);
		_bstr_t bstrSubject(pICertInfo->GetSubject(), false);

		// 申告等データファイルへの署名付与
		_bstr_t bstrXML       = "..\\Data\\RHO0010.xtx";
		_bstr_t bstrSignedXML = "..\\Data\\RHO0010signedIcc.xtx";
		hr = pISignature->SignToReport(bstrXML, bstrSignedXML, "", "1777187306200700");
		if (FAILED(hr)) {
			_com_raise_error(hr);
		}

	// COM例外ハンドラ
	} catch(_com_error e) {
		MessageBox(0, e.Description(), e.ErrorMessage(), MB_OK);
		throw;
	}
}

void AddSignatureToCertRegistICCard()
{
	// ネームスペース宣言
	using namespace CLXTXSIGNERLib;

	try {

		// 署名モジュールインスタンス生成
		CLISignaturePtr pISignature;
		HRESULT hr = pISignature.CreateInstance(__uuidof(CLCXtxSigner));
		if (FAILED(hr)) {
			_com_raise_error(hr);
		}

		// ICカード接続（新ICカード）
		_bstr_t bstrCSPName("JPKI Crypto Service Provider");// 公的個人認証サービスのCSPを利用する場合
		CLICertificateInfoPtr pICertInfo = pISignature->SetCertificateICCard(bstrCSPName);

		// 有効期間チェック
		_variant_t varValidity = pICertInfo->IsValid();
		if (varValidity.boolVal == VARIANT_FALSE) {
			MessageBox(0, "有効期間切れ", "署名モジュール", MB_OK);
			_com_raise_error(E_INVALIDARG);
		}

		// 電子証明書更新申請への新電子証明書での署名付与
		_bstr_t bstrXML       = "..\\Data\\PTE0010.xtx";
		_bstr_t bstrSignedXML = "..\\Data\\PTE0010signedIcc.xtx";
		hr = pISignature->SignToCertificateRegistration(bstrXML, bstrSignedXML, 1, "", "1777187306200700");
		if (FAILED(hr)) {
			_com_raise_error(hr);
		}

		//（例）ユーザにカードの差し替えを促すための表示
		MessageBox(NULL, "旧ICカードに差換えて下さい", "カード差換え", MB_OK|MB_ICONINFORMATION); 

		// ICカード接続（旧ICカード）
		bstrCSPName = "JPKI Crypto Service Provider";// 公的個人認証サービスのCSPを利用する場合
		pICertInfo = pISignature->SetCertificateICCard(bstrCSPName);

		// 有効期間チェック
		varValidity = pICertInfo->IsValid();
		if (varValidity.boolVal == VARIANT_FALSE) {
			MessageBox(0, "有効期間切れ", "署名モジュール", MB_OK);
			_com_raise_error(E_INVALIDARG);
		}

		// 電子証明書更新申請への旧電子証明書での署名付与
		bstrXML       = "..\\Data\\PTE0010signedIcc.xtx";
		bstrSignedXML = "..\\Data\\PTE0010signedIcc.xtx";
		hr = pISignature->SignToCertificateRegistration(bstrXML, bstrSignedXML, 2, "", "1777187306200700");
		if (FAILED(hr)) {
			_com_raise_error(hr);
		}

	// COM例外ハンドラ
	} catch(_com_error e) {
		MessageBox(0, e.Description(), e.ErrorMessage(), MB_OK);
		throw;
	}
}
