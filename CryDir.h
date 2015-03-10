#pragma once
#ifndef CRYFS_LIB_CRYDIR_H_
#define CRYFS_LIB_CRYDIR_H_

#include <messmer/fspp/fs_interface/Dir.h>
#include "CryNode.h"
#include "impl/DirBlob.h"

namespace cryfs {

class CryDir: public fspp::Dir, CryNode {
public:
  CryDir(CryDevice *device, const blockstore::Key &key);
  virtual ~CryDir();

  void stat(struct ::stat *result) const override;
  //TODO return type variance to CryFile/CryDir?
  std::unique_ptr<fspp::OpenFile> createAndOpenFile(const std::string &name, mode_t mode) override;
  void createDir(const std::string &name, mode_t mode) override;
  void rmdir() override;

  //TODO Make Entry a public class instead of hidden in DirBlob (which is not publicly visible)
  std::unique_ptr<std::vector<fspp::Dir::Entry>> children() const override;

private:
  CryDevice *_device;
  blockstore::Key _key;

  std::unique_ptr<DirBlob> LoadBlob() const;

  DISALLOW_COPY_AND_ASSIGN(CryDir);
};

}

#endif
