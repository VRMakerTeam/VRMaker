#include <QNetworkInterface>
#include <QCryptographicHash>
#include "AppUtil.h"


QString getCPUID1() {
	char OEMString[13];
	QString result = QString("");
	int iEAXValue, iEBXValue, iECXValue, iEDXValue;
	_asm
	{
		mov eax, 0
		cpuid
		mov DWORD PTR OEMString, ebx
		mov DWORD PTR OEMString + 4, edx
		mov DWORD PTR OEMString + 8, ecx
		mov BYTE PTR OEMString + 12, 0
	}
	// SetDlgItemText(IDC_STATIC1,OEMString); //CPU 供应商 名称
	qDebug() << "manufacture id is " << QString(OEMString);
	_asm
	{
		mov eax, 1
		cpuid
		mov iEAXValue, eax
		mov iEBXValue, ebx
		mov iECXValue, ecx
		mov iEDXValue, edx
	}
	int iCPUFamily = (0xf00 & iEAXValue) >> 8;
	char Family[10] = { 0 };
	_itoa_s(iCPUFamily, Family, 10);
	// SetDlgItemText(IDC_STATIC2,Family); //CPU系列
	qDebug() << "cpu family is " << QString(Family);
	_asm
	{
		mov eax, 2
		CPUID
	}
	char szCPUID[129] = { NULL };
	char szTmp[33] = { NULL };
	unsigned long s1 = 0, s2 = 0;
	_asm
	{
		mov eax, 01h
		xor edx, edx
		cpuid
		mov s1, edx
		mov s2, eax
	}
	//大写16 进制，宽度占8 个位置，右对齐
	sprintf_s(szTmp, "%08X%08X", s1, s2);
	strcpy_s(szCPUID, szTmp);
	_asm
	{
		mov eax, 03h
		xor ecx, ecx
		xor edx, edx
		cpuid
		mov s1, edx
		mov s2, ecx
	}
	sprintf_s(szTmp, "%08X%08X", s1, s2);
	strcat_s(szCPUID, szTmp);
	// SetDlgItemText(IDC_STATIC3,szCPUID); //CPUID号
	qDebug() << "cpuid2 is " << QString(szCPUID);
	// ui->textBrowser_5->append(tr("\r\n cpuid1 is %1").arg(QString(szCPUID)));
	result = QString(szCPUID).toUpper();
	return result;
}

QString getCPUID2() {
	unsigned long dwId1, dwId2, dwId3, dwId4;
	char szCompany[13];
	char* pCompany = szCompany;
	szCompany[12] = 0;
	_asm
	{
		pushfd
		pushad
		//取得CPU的ID号
		mov eax, 1 //功能号
		_emit 0x0f
		_emit 0xa2
		mov dwId1, eax
		mov dwId2, ebx
		mov dwId3, ecx
		mov dwId4, edx
		//取得CPU的制造公司名称
		mov edi, pCompany //功能号
		mov eax, 0
		_emit 0x0f
		_emit 0xa2
		mov eax, ebx
		stosd
		mov eax, edx
		stosd
		mov eax, ecx
		stosd
		popad
		popfd
	}
	unsigned long dwResult = 0;
	unsigned long dwTemp1 = dwId1 << 12;
	unsigned long dwTemp2 = dwId2 << 8;
	unsigned long dwTemp3 = dwId3 << 4;
	QString res = QString("splitted string is %1_%2_%3_%4").arg(QString::number(dwTemp1, 16)).arg(QString::number(dwTemp2, 16)).arg(QString::number(dwTemp3, 16)).arg(QString::number(dwId4, 16));
	dwResult = dwTemp1 + dwTemp2 + dwTemp3 + dwId4;
	QString result = QString::number(dwResult, 16).toUpper();
	qDebug() << "the result is " << result;
	QString cpy = QString::fromLocal8Bit(szCompany);
	// ui->textBrowser_5->append(res+tr("\r\n cpuid2 is :")+result.toUpper()+tr("\r\n company is :")+cpy);
	return result;
}

QString AppUtil::UUID()
{
	QString addr = QString("%1%2").arg(getCPUID1()).arg(getCPUID2());
	return QString(QCryptographicHash::hash(addr.toUtf8(), QCryptographicHash::Md5).toHex());
}
