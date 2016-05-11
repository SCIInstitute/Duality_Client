
#pragma once

#ifndef GPUOBJECT_H
#define GPUOBJECT_H

/** Base for all types which will utilize GPU memory.
 * Forces children to keep track of memory use on both the CPU and GPU,
 * allowing us to use as much memory as will be possible without swapping. */
class GPUObject {
public:
  virtual ~GPUObject() {}
  virtual uint64_t GetCPUSize() = 0;
  virtual uint64_t GetGPUSize() = 0;
};

#endif // GPUOBJECT_H
