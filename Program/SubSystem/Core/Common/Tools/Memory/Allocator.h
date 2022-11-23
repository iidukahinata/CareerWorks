/**
* @file    Allocator.h
* @brief   
*
* @date	   2022/07/19 2022年度初版
*/
#pragma once


typedef unsigned long ulong;
class BoundaryBlock;

class Allocator
{
public:

	Allocator(uint32_t memorySize);
	~Allocator();

	void* Allocate(uint32_t size) noexcept;
	void Deallocate(void* ptr) noexcept;

private:

	ulong GetMsb(const uint32_t num) const noexcept;
	ulong GetLsb(const uint32_t num) const noexcept;

	ulong GetSecondCategory(const uint32_t num, const uint32_t msb) const noexcept;

	/** 
	* フリーリストに bit 以上のメモリサイズを保持するかを判別。
	* @param listBit には FLI もしくは SLI Bit列を指定してください。
	* @return フリーリストが存在する場合その ID を返す。
	*/
	uint8_t GetFreeListToCategory(const uint8_t bit, const uint32_t listBit) const noexcept;

	/**
	* 引数値からフリーリストオブジェクトを検索し取得。
	* もし指定オブジェクトが無い場合、更に大きなメモリを検索しにいく。
	*/
	BoundaryBlock* GetFreeList(uint32_t fli, uint32_t sli) const noexcept;

	/** フリーリスト操作 */
	void AddToFreeList(BoundaryBlock* block) noexcept;
	void RemoveFromFreeList(BoundaryBlock* block) noexcept;

private:

	const uint32_t N = 4;

	// * 第二カテゴリの分割数
	uint32_t m_numOfDivisions /* = pow(2, N) */;

	// * 確保したPoolの先頭ポインタ
	void* m_memory = nullptr;

	uint32_t m_maxMemorySize;

	// * 第一カテゴリの所持メモリフラグ
	uint32_t m_globalFLI = 0;

	// * 第二カテゴリの所持メモリフラグ
	uint32_t* m_freeListBits;

	// * 割り振り可能なフリーメモリリスト
	BoundaryBlock** m_freeLists;
};