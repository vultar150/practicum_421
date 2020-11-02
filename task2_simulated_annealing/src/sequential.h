#ifndef SEQUENTIAL_H
#define SEQUENTIAL_H


template<typename Dec, typename Mut, typename Law, typename T>
class SequentialAlg
{
    protected:
        std::shared_ptr<AbstractTypeDecision<T>> _decision;
        std::shared_ptr<AbstractTypeDecision<T>> _record;
        std::unique_ptr<AbstractMutation<T>>     _mutation;
        std::unique_ptr<AbstractTDecreaseLaw>    _law;
        double _T;

    public:

        SequentialAlg(char* fileName, double T0=1.) : 
        _decision(std::make_shared<Dec>(fileName)),
        _record(_decision), _mutation(std::make_unique<Mut>()),
        _law(std::make_unique<Law>(T0)), _T(T0) {}


        std::shared_ptr<AbstractTypeDecision<T>> startMainCycle()
        {
            if (_decision->getData().size() <= 1) return _record;
            int it = 1, itWithoutChange = 0, itT = 1;
            std::shared_ptr<AbstractTypeDecision<T>> newDecision;

            while (itWithoutChange <= NUM_OF_IT_INNER_MOST_LOOP_WITHOUT_CHANGE)
            {
                while (itT <= NUM_OF_IT_WITHOUT_CHANGE_TEMPERATUE and 
                       itWithoutChange <= NUM_OF_IT_INNER_MOST_LOOP_WITHOUT_CHANGE)
                {
                    newDecision = _mutation->mutate(_decision);
                    if (doChangeDecision(newDecision))
                    {
                        _decision = newDecision;
                        if (_record->targetFunc() > _decision->targetFunc()) // criterion
                        {
                            _record = _decision;
                            itWithoutChange = 0;
                        }
                    }
                    itWithoutChange++;
                    itT++;
                }
                std::cout << "///////////TEMPERATURE = " << _T << std::endl;
                _T = _law->getDecreaseTemperature(it);
                it++;
                itT = 1;
            }
            std::cout << "NUM OF IT = " << it << std::endl;
            std::cout << "NUM OF IT WITHOUT CHANGE = " << itWithoutChange << std::endl;
            std::cout << "TEMPERATURE = " << _T << std::endl;
            return _record;



            // int it = 1, itWithoutChange = 0;
            // std::shared_ptr<AbstractTypeDecision<T>> newDecision;

            // while (itWithoutChange <= NUM_OF_IT_INNER_MOST_LOOP_WITHOUT_CHANGE)
            // {
            //     newDecision = _mutation->mutate(_decision);
            //     if (doChangeDecision(newDecision))
            //     {
            //         _decision = newDecision;
            //         if (_record->targetFunc() > _decision->targetFunc()) // criterion
            //         {
            //             _record = _decision;
            //             itWithoutChange = 0;
            //         }
            //         else itWithoutChange++;
            //     }
            //     else itWithoutChange++;
            //     if (not itWithoutChange % NUM_OF_IT_WITHOUT_CHANGE_TEMPERATUE 
            //         and not itWithoutChange)
            //     {
            //             _T = _law->getDecreaseTemperature(it);
            //             it++;
            //     }
            // }
            // return _record;
        }

        bool doChangeDecision(std::shared_ptr<AbstractTypeDecision<T>> newD)
        {
            int Delta = 0;
            Delta = newD->targetFunc() - _decision->targetFunc(); // criterion
            // Delta = _decision->targetFunc() - newD->targetFunc();
            if (Delta <= 0) return true;
            else 
            {
                int p = round(exp(-Delta / _T) * 100);
                std::cout << "DELTA = " << Delta << std::endl;
                std::cout << "TEMPERATURE = " << _T << std::endl;
                std::cout << "P = " << p << std::endl;
                int randv = 1 + arc4random_uniform(100);
                if (randv <= p) return true;
                else            return false;
            }
        }
};


#endif // SEQUENTIAL_H