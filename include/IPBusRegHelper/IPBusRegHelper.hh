#ifndef __IPBUS_REG_HELPER_HH__
#define __IPBUS_REG_HELPER_HH__

#include <BUTool/helpers/register_helper.hh>
#include <IPBusIO/IPBusIO.hh>


class IPBusRegHelper:  public IPBusIO, public BUTool::RegisterHelper{
  //This class is a bridge to connect the API for the BUTool reg helpers with the IPBus IO functions.
  //This gets its API from the RegisterHelper so that BUTool gets a consistant set of commands for free.
  //The implementation of these functions is taken from the IPBusIO class, but for the inheritance to work, we 
  //need to make local overloads of the RegisterHelper API that call the equivalent functions from IPBusIO.
  //This means that the function names and signatures from IPBusIO don't have to match RegisterHelper, 
  //but let's make life easier and say the must be the same
public:
  std::vector<std::string> myMatchRegex(std::string regex){return IPBusIO::myMatchRegex(regex);};  
  //Misc markups
  uint32_t    GetRegAddress(std::string const & reg){return IPBusIO::GetRegAddress(reg);};
  uint32_t    GetRegMask(std::string const & reg){return IPBusIO::GetRegMask(reg);};
  uint32_t    GetRegSize(std::string const & reg){return IPBusIO::GetRegSize(reg);};
  std::string GetRegMode(std::string const & reg){return IPBusIO::GetRegMode(reg);};
  std::string GetRegPermissions(std::string const & reg){return IPBusIO::GetRegPermissions(reg);};
  std::string GetRegDescription(std::string const & reg){return IPBusIO::GetRegDescription(reg);};
  std::string GetRegDebug(std::string const & reg){return IPBusIO::GetRegDebug(reg);};  


  uint32_t RegReadAddress(uint32_t addr){return IPBusIO::RegReadAddress(addr);};
  //Named register reads
  uint32_t RegReadRegister(std::string const & reg){return IPBusIO::RegReadRegister(reg);};
  std::string RegReadString(std::string const & reg){
    uhal::Node const & node = IPBusIO::GetNode(reg);
    uhal::defs::BlockReadWriteMode mode = node.getMode();
    size_t stringLen=IPBusIO::GetRegSize(reg);
    //make sure the mode is non_incremental 
    if((uhal::defs::INCREMENTAL != mode) &&
       (!stringLen)){
      BUException::REG_READ_DENIED e2;    
      e2.Append(reg);
      e2.Append("is not a string\n");
      throw e2;
    }
    std::vector<uint32_t> val = RegBlockReadRegister(reg,stringLen);
    std::string ret( (char *) val.data(),stringLen);
    return ret;
  }

  void RegWriteAddress(uint32_t addr, uint32_t data){IPBusIO::RegWriteAddress(addr,data);};
  //Named register writes
  void RegWriteRegister(std::string const & reg, uint32_t data){IPBusIO::RegWriteRegister(reg,data);};
  void RegWriteAction(std::string const & reg){IPBusIO::RegWriteAction(reg);};

};
#endif
