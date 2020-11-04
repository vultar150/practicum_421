#ifndef SEQUENTIAL_H
#define SEQUENTIAL_H


template<typename Dec, typename Mut, typename Law, typename T>
class SequentialAlg
{
    protected:
        bool _isMin;
        double _T;
        std::shared_ptr<AbstractTypeDecision<T>> _decision;
        std::shared_ptr<AbstractTypeDecision<T>> _record;
        std::unique_ptr<AbstractMutation<T>>     _mutation;
        std::unique_ptr<AbstractTDecreaseLaw>    _law;
        std::unordered_map<std::string, int>&    _params;

    public:

        SequentialAlg(std::shared_ptr<AbstractTypeDecision<T>> decision,
                      std::unordered_map<std::string, int>&    params) :
                      _isMin(params["IS_MIN_CRITERION"]),
                      _T(params["INITIAL_TEMPERATURE"]),
                      _decision(decision), _record(_decision),
                      _mutation(std::make_unique<Mut>(_isMin)),
                      _law(std::make_unique<Law>(_T)), _params(params) {}


        std::shared_ptr<AbstractTypeDecision<T>> startMainCycle()
        {
            if (_decision->getData().size() <= 1) return _record;
            int it = 1, itWithoutChange = 0, itT = 1;
            std::shared_ptr<AbstractTypeDecision<T>> newDecision;

            while (itWithoutChange <= _params["NUM_OF_IT_INNER_MOST_LOOP_WITHOUT_CHANGE"])
            {
                while (itT <= _params["NUM_OF_IT_WITHOUT_CHANGE_TEMPERATUE"] and 
                       itWithoutChange <= _params["NUM_OF_IT_INNER_MOST_LOOP_WITHOUT_CHANGE"])
                {
                    newDecision = _mutation->mutate(_decision);
                    if (doChangeDecision(newDecision))
                    {
                        _decision = newDecision;
                        int delta = (_isMin) ? 
                                    _record->targetFunc() - _decision->targetFunc() :
                                    _decision->targetFunc() - _record->targetFunc();
                        if (delta > 0)
                        {
                            _record = _decision;
                            itWithoutChange = 0;
                        }
                    }
                    itWithoutChange++;
                    itT++;
                }
                _T = _law->getDecreaseTemperature(it);
                it++;
                itT = 1;
            }
            return _record;


            // if (_decision->getData().size() <= 1) return _record;
            // int it = 1, itWithoutChange = 0;
            // std::shared_ptr<AbstractTypeDecision<T>> newDecision;

            // while (itWithoutChange <= _params["NUM_OF_IT_INNER_MOST_LOOP_WITHOUT_CHANGE"])
            // {
            //     newDecision = _mutation->mutate(_decision);
            //     if (doChangeDecision(newDecision))
            //     {
            //         _decision = newDecision;
            //         int delta = (_isMin) ? 
            //                     _record->targetFunc() - _decision->targetFunc() :
            //                     _decision->targetFunc() - _record->targetFunc();
            //         if (delta > 0)
            //         {
            //             _record = _decision;
            //             itWithoutChange = 0;
            //         }
            //     }
            //     itWithoutChange++;
            //     if (not itWithoutChange % (_params["NUM_OF_IT_WITHOUT_CHANGE_TEMPERATUE"] + 1))
            //     {
            //         _T = _law->getDecreaseTemperature(it);
            //         it++;
            //     }
            // }
            // return _record;
        }


        bool doChangeDecision(std::shared_ptr<AbstractTypeDecision<T>> newD)
        {
            if (_decision != newD)
            {
                int Delta = (_isMin) ? 
                            newD->targetFunc() - _decision->targetFunc() :
                            _decision->targetFunc() - newD->targetFunc();
                if (Delta <= 0) return true;
                else 
                {
                    int p = round(exp(-Delta / _T) * 100);
                    int randv = 1 + arc4random_uniform(100);
                    if (randv <= p) return true;
                    else            return false;
                }
            }
            else return false;
        }


        void print()
        {
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            std::cout << "CURR DECISION = " << _decision->targetFunc() << std::endl;
            std::cout << "CURR RECORD = " << _record->targetFunc() << std::endl;
            std::cout << "EXECUTION TIME (SUM) = " << _record->getExecTime() << std::endl;
            std::cout << "ID MAX PROC = " << _record->getIdMaxProc() << std::endl;
            std::cout << "ID MIN PROC = " << _record->getIdMinProc() << std::endl;
            // std::cout << "ID MIN PROC = " << _record->getIdMinProc() << std::endl;
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
        }
};


#endif // SEQUENTIAL_H