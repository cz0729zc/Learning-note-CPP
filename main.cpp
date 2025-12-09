#include <iostream>
#include <memory>


class Entity
{
public:
    Entity()
    {
        std::cout << "Created Entity" << std::endl;
    }

    ~Entity()
    {
        std::cout << "Destroyed Entity" << std::endl;
    }

    void print()
    {
        std::cout << "Hello Entity" << std::endl;
    }

};



/*
 * 使用智能指针原则：优先使用unique_ptr，如果需要共享所有权则使用shared_ptr，如果不需要所有权则使用weak_ptr。 
 */    
int main()
{
    // 使用标准库中的unique_ptr智能指针
    // {
    //     /* 需要显式构造 */
    //     // std::unique_ptr<Entity> entity(new Entity()); 

    //     /* 不能这样写，编译错误 */ 
    //     // std::unique_ptr<Entity> entity = new Entity(); 

    //     /* 或者使用make_unique（推荐）  */  
    //     // std::unique_ptr<Entity> entity = std::make_unique<Entity>();

    //     /* unique_ptr不支持拷贝，所以下面的代码会编译错误，因为不能有两个unique_ptr指向同一个对象，如果当一个unique_ptr被销毁时，另一个unique_ptr还在使用这个对象，就会导致悬空指针问题。*/ 
    //     // std::unique_ptr<Entity> entity2 = entity;

    //     /* 使用方式和普通指针一样 */
    //     // entity->print();
    // }

    // 使用标准库中的shared_ptr智能指针
    {
        // shared_ptr支持拷贝,每拷贝一次引用计数加1,当引用计数为0时才会释放内存
        // std::shared_ptr<Entity> sharedEntity2;

        // 如果不想要Entity的所有权，可以使用weak_ptr，这样不会增加引用计数
        std::weak_ptr<Entity> weakEntity;
        {
            /* 如果需要拷贝的话可以使用shared_ptr */
            std::shared_ptr<Entity> sharedEntity = std::make_shared<Entity>();
            // sharedEntity2 = sharedEntity; // 这样是可以
            weakEntity = sharedEntity; // weak_ptr可以从shared_ptr构造
        }        
    }


    return 0;
}