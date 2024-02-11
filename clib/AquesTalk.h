//////////////////////////////////////////////////////////////////////
/*!	@class	AquesTalk

	@brief	規則音声合成エンジン AquesTalk10

  音声記号列から音声波形データをメモリ上に生成する
  出力音声波形は、16KHz, 16bit,モノラル,WAVフォーマット。
                  *サンプリング周波数は、声種パラメータfscにより変化

	@author	N.Yamazaki (AQUEST)

	@date	2017/10/31	N.Yamazaki	Creation for Linux
*/
//////////////////////////////////////////////////////////////////////
#if !defined(_AQUESTALK10_H_)
#define _AQUESTALK10_H_
#ifdef __cplusplus
extern "C"{
#endif

// 声質パラメータ
typedef struct _AQTK_PARAM_ {
	int bas;	// 基本素片 F1E/F2E/M1E (0/1/2)
	int spd;	// 話速 	50-300 default:100
	int vol;	// 音量 	0-300 default:100
	int pit;	// 高さ 	20-200 default:基本素片に依存
	int acc;	// アクセント 0-200 default:基本素片に依存
	int lmd;	// 音程１ 	0-200 default:100
	int fsc;	// 音程２(サンプリング周波数) 50-200 default:100
} AQTK_VOICE;

enum VoiceBase { F1E =0, F2E, M1E }; // 基本素片

// プリセット声種                 bas  spd  vol  pit  acc  lmd  fsc
const AQTK_VOICE	gVoice_F1 = { F1E, 100, 100, 100, 100, 100, 100}; // 女声 F1
const AQTK_VOICE	gVoice_F2 = { F2E, 100, 100,  77, 150, 100, 100}; // 女声 F2
const AQTK_VOICE	gVoice_F3 = { F1E,  80, 100, 100, 100,  61, 148}; // 女声 F3
const AQTK_VOICE	gVoice_M1 = { M1E, 100, 100,  30, 100, 100, 100}; // 男声 M1
const AQTK_VOICE	gVoice_M2 = { M1E, 105, 100,  45, 130, 120, 100}; // 男声 M2
const AQTK_VOICE	gVoice_R1 = { M1E, 100, 100,  30,  20, 190, 100}; // ロボット R1
const AQTK_VOICE	gVoice_R2 = { F2E,  70, 100,  50,  50,  50, 180}; // ロボット R2


/////////////////////////////////////////////
//!	音声記号列から音声波形を生成
//!	音声波形データは内部で領域確保される。
//!	音声波形データの解放は本関数の呼び出し側でAquesTalk_FreeWave()にて行うこと
//!
//! @param	pParam[in]	声質パラメータ (AQTK_VOICE構造体のアドレス)
//! @param	koe[in]		音声記号列（SJIS NULL終端）
//!	@param	pSize[out]	生成した音声データのサイズ[byte]（エラーの場合はエラーコードが返る）
//!	@return	WAVフォーマットの音声データの先頭アドレス。エラー時はNULLが返る
unsigned char * AquesTalk_Synthe(const AQTK_VOICE *pParam, const char *koe, int *pSize);

//! @param	koe[in]		音声記号列（UTF8 NULL終端 BOM無し）
unsigned char * AquesTalk_Synthe_Utf8(const AQTK_VOICE *pParam, const char *koe, int *pSize);

//! @param	koe[in]		音声記号列（UTF16 NULL終端 BOMの有無は問わない　エンディアンは実行環境に従う）
unsigned char * AquesTalk_Synthe_Utf16(const AQTK_VOICE *pParam, const unsigned short *koe, int *pSize);


/////////////////////////////////////////////
//!	音声データの領域を開放
//!	@param  wav[in]		AquesTalk_Synthe()で返されたアドレスを指定
void AquesTalk_FreeWave(unsigned char *wav);


/////////////////////////////////////////////
//!	開発ライセンスキー設定
//!	音声波形を生成する前に一度呼び出す。
//!	これにより評価版の制限がなくなる。
//!	@param  key[in]		ライセンスキーを指定
//!	@return	ライセンスキーが正しければ0、正しくなければ1が返る
//! *キーの解析を防ぐため不正なキーでも0を返す場合がある。このとき制限は解除されない。
int AquesTalk_SetDevKey(const char *key);

/////////////////////////////////////////////
//!	使用ライセンスキー設定
//!	音声波形を生成する前に一度呼び出す。
//!	以降、合成音声データに含まれる透かしが、使用ライセンス無しから取得済みに変化する
//!	@param  key[in]		ライセンスキーを指定
//!	@return	ライセンスキーが正しければ0、正しくなければ1が返る
//! *キーの解析を防ぐため不正なキーでも0を返す場合がある。この場合、ライセンス無のままである。
int AquesTalk_SetUsrKey(const char *key);

#ifdef __cplusplus
}
#endif
#endif // !defined(_AQUESTALK10_H_)
//  ----------------------------------------------------------------------
// !  Copyright AQUEST Corp. 2006- .  All Rights Reserved.                !
// !  An unpublished and CONFIDENTIAL work.  Reproduction, adaptation, or !
// !  translation without prior written permission is prohibited except   !
// !  as allowed under the copyright laws.                                !
//  ----------------------------------------------------------------------
