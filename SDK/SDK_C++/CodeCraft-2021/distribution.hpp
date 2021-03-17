#ifndef __DISTRIBUTION_H
#define __DISTRIBUTION_H 

#include <vector>

#include <algorithm>
#include <unordered_set>
#include "server.hpp"
#include "virtualMachine.hpp"
#include "tools.hpp"
using namespace std;


// 返回不同的操作
enum distribution_type{
    norm = 0,// 正常分配操作
    erase = 1,// 删除服务器操作
    add = 2// 添加服务器操作 
};

// 正常分配的信息
struct  distribution_operation
{
    int server_id;// 分配到id对应的服务器上
    int server_type;//只有当额外添加服务器时才用
    int node_type;//节点的类型，放置在A节点上还是B节点上，或者双节点AB
    // norm 正常操作 
    // erase 将 server_id 对应的服务器删除 
    // add 则是再添加一个server_type类型的服务器，其id为 server_id 
    int distribution_type;
    distribution_operation()
    {
    }
    distribution_operation(const distribution_operation & op)
    {
        server_id = op.server_id;
        server_type = op.server_type;
        node_type = op.node_type;
        distribution_type = op.distribution_type;
    }
};


class distribution
{
public:
    distribution(std::vector<server_data>& servers, std::vector<virtual_machine_data>& VMs);
    ~distribution();
    // 数据接口，尝试根据task的任务将虚拟机往服务器上分配
    std::vector<distribution_operation> try_distribution(
        std::vector<int>& servers_type_id,
        std::vector<std::vector<int> >& VMs_type_id,
        task task_today,
        std::vector<int>& remain_CPU_A,
        std::vector<int>& remain_RAM_A,
        std::vector<int>& remain_CPU_B,
        std::vector<int>& remain_RAM_B
    );
std::vector<distribution_operation> try_violence_distribution(
        std::vector<int> servers_type_id,
        std::vector<std::vector<int> > VMs_type_id,
        task tasks,
        std::vector<int> left_CPU_A,
        std::vector<int> left_RAM_A,
        std::vector<int> left_CPU_B,
        std::vector<int> left_RAM_B);
private:    
    /************ used for violent distribution ***************/
    void sort_Server_Index();// 对服务器表进行排序
    /********************************************************/
private:
    /* data */    
    // 保存所有已有的虚拟机和服务器的实例表：相当于产品表
    std::vector<server_data> m_servers;// 目前是按照读入顺序
    std::vector<virtual_machine_data> m_VMs;// 目前按照读入顺序排序
    std::vector<distribution_operation> distribution_result_queue;//当天操作完成后返回的操作数据队列
    std::vector<distribution_operation> server_buy_queue;//当天操作完成后返回的操作数据队列

    std::vector<int> sorted_server_table;// 按照约定排序方式排序好的服务器表，存的元素是对应型号的索引
    std::vector<int> sorted_vm_table;// 按照约定排序方式排序好的虚拟机表，存的元素是对应型号的索引

    std::vector<int> split_pos;//存放del命令在今日命令中的索引
    const int num_of_server_type = 105;// 服务器的最大种类数

    float weight_cpu = 0.5;
    float weight_ram = 0.5;
};

#endif // __DISTRIBUTION_H
