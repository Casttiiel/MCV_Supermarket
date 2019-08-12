#pragma once

// -------------------------------------------------------
class CGPUBuffer {

public:
  bool create(uint32_t bytes_per_elem, uint32_t num_elems, bool is_indirect);

  // To hold the data in the cpu
  std::vector<uint8_t>         cpu_data;

  bool copyGPUtoCPU() const;
  void copyCPUtoGPUFrom(const void* new_data) const;
  bool copyCPUtoGPU() const;
  bool create(const json& j);

  ID3D11Buffer*                buffer = nullptr;
  ID3D11ShaderResourceView*    srv = nullptr;
  ID3D11UnorderedAccessView*   uav = nullptr;
  uint32_t                     bytes_per_elem = 0;
  uint32_t                     num_elems = 0;
  std::string                  name;
  void destroy();
};

// -------------------------------------------------------
template< typename T>
struct TGPUTypedBuffer : public CGPUBuffer {
  
  bool create(uint32_t new_num_elems) {
    return CGPUBuffer::create(sizeof(T), new_num_elems);
  }

  // return typed data of the first elem
  T* data() {
    return (T*)cpu_data.data();
  }

  size_t size() const { return num_elems; }
};

