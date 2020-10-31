#ifndef MUTATION_H
#define MUTATION_H


// Mutation operation

template<typename T>
class AbstractMutation
{
    public:

        virtual int getIdProcessorFrom(std::shared_ptr<AbstractTypeDecision<T>> decision) const=0;
        virtual int getIdProcessorTo(std::shared_ptr<AbstractTypeDecision<T>> decision) const=0;
        virtual int getRandomJobId(std::shared_ptr<AbstractTypeDecision<T>> decision, int from) const=0;
        virtual std::shared_ptr<AbstractTypeDecision<T>> mutate(std::shared_ptr<AbstractTypeDecision<T>> decision)=0;

        virtual ~AbstractMutation()=default;
};


class MyOperation: public AbstractMutation<MyDataType>
{
    public:
        virtual int getIdProcessorFrom(std::shared_ptr<AbstractTypeDecision<MyDataType>> decision) const override;
        virtual int getIdProcessorTo(std::shared_ptr<AbstractTypeDecision<MyDataType>> decision) const override;
        virtual int getRandomJobId(std::shared_ptr<AbstractTypeDecision<MyDataType>> decision, int from) const override;
        virtual std::shared_ptr<AbstractTypeDecision<MyDataType>> mutate(std::shared_ptr<AbstractTypeDecision<MyDataType>> decision) override;
};

// end mutation operation


#endif // MUTATION_H