#ifndef MUTATION_H
#define MUTATION_H


// Mutation operation

template<typename T>
class AbstractMutation
{
    public:

        virtual std::shared_ptr<AbstractTypeDecision<T>> 
        mutate(std::shared_ptr<AbstractTypeDecision<T>> decision)=0;
        
        virtual AbstractMutation<T>& operator=(AbstractMutation<T>& mut)=0;
        virtual bool isMin() const=0;
        virtual ~AbstractMutation()=default;
};


class MyOperation: public AbstractMutation<MyDataType>
{
    private:
        bool _isMin;

    public:
        MyOperation(bool isMin=true);

        virtual AbstractMutation<MyDataType>& 
        operator=(AbstractMutation<MyDataType>& mut) override;

        virtual bool isMin() const override;

        virtual std::shared_ptr<AbstractTypeDecision<MyDataType>> 
        mutate(std::shared_ptr<AbstractTypeDecision<MyDataType>> decision) override;

        std::shared_ptr<AbstractTypeDecision<MyDataType>>
        minCriterion(std::shared_ptr<AbstractTypeDecision<MyDataType>> decision);

        std::shared_ptr<AbstractTypeDecision<MyDataType>>
        maxCriterion(std::shared_ptr<AbstractTypeDecision<MyDataType>> decision);

        template<typename TCont>
        int getRandomIdPositive(TCont& cont, uint64_t execTime);

        template<typename TCont>
        int getRandomIdNegative(TCont& cont, uint64_t execTime, bool shouldNotBeEmpty=false);
};


// end mutation operation


#endif // MUTATION_H