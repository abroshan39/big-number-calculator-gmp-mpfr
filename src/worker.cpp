/*
    [Big Number Calculator]
    Publisher: Rosybit
    Url: http://www.rosybit.com
    GitHub: https://github.com/abroshan39/big-number-calculator-gmp-mpfr
    Author: Aboutaleb Roshan [ab.roshan39@gmail.com]
    License: MIT License
*/

#include "worker.h"
#include "gmp.h"
#include "mpfr.h"

Worker::Worker(AppMode appMode, NumType numType, OpType opType, const QString &n1Str, int n1Base, const QString &n2Str, int n2Base, int resultBase, long int precision, RoundMode roundMode)
{
    this->appMode = appMode;
    this->numType = numType;
    this->opType = opType;
    this->n1Str = n1Str;
    this->n2Str = n2Str;
    this->n1Base = n1Base;
    this->n2Base = n2Base;
    this->resultBase = resultBase;
    this->precision = precision;
    this->roundMode = roundMode;
}

void Worker::process()
{
    QString result_str;

    if(appMode == AppMode::Calc)
    {
        if(numType == NumType::Integer)
        {
            char *str;
            mpz_t n1, n2, result;

            QByteArray byteArray1 = n1Str.toLocal8Bit();
            QByteArray byteArray2 = n2Str.toLocal8Bit();
            const char *n1_char_array = byteArray1.constData();
            const char *n2_char_array = byteArray2.constData();

            mpz_inits(n1, n2, result, NULL);
            mpz_set_str(n1, n1_char_array, n1Base);
            mpz_set_str(n2, n2_char_array, n2Base);

            if(!mpz_cmp_ui(n2, 0) && (opType == OpType::Div || opType == OpType::Mod || opType == OpType::Root))
            {
                emit finished("*Undefined*");
                return;
            }

            if(opType == OpType::Add)
                mpz_add(result, n1, n2);
            else if(opType == OpType::Sub)
                mpz_sub(result, n1, n2);
            else if(opType == OpType::Mul)
                mpz_mul(result, n1, n2);
            else if(opType == OpType::Div)
                mpz_fdiv_q(result, n1, n2);
            else if(opType == OpType::Mod)
                mpz_mod(result, n1, n2);
            else if(opType == OpType::Pow)
                mpz_pow_ui(result, n1, mpz_get_ui(n2));
            else if(opType == OpType::Root)
                mpz_root(result, n1, mpz_get_ui(n2));

            str = mpz_get_str(NULL, resultBase, result);
            result_str = str;

            free(str);
            mpz_clears(n1, n2, result, NULL);
        }
        else if(numType == NumType::FloatingPoint)
        {
            char *str;
            mpfr_t n1, n2, result;
            mpfr_rnd_t rnd_t = MPFR_RNDN;
            mpfr_prec_t prec_t = precision;

            if(roundMode == RoundMode::RNDN)
                rnd_t = MPFR_RNDN;
            else if(roundMode == RoundMode::RNDD)
                rnd_t = MPFR_RNDD;
            else if(roundMode == RoundMode::RNDU)
                rnd_t = MPFR_RNDU;
            else if(roundMode == RoundMode::RNDZ)
                rnd_t = MPFR_RNDZ;
            else if(roundMode == RoundMode::RNDA)
                rnd_t = MPFR_RNDA;

            if(prec_t < MPFR_PREC_MIN)
                prec_t = MPFR_PREC_MIN;
            else if(prec_t > MPFR_PREC_MAX)
                prec_t = MPFR_PREC_MAX;

            QByteArray byteArray1 = n1Str.toLocal8Bit();
            QByteArray byteArray2 = n2Str.toLocal8Bit();
            const char *n1_char_array = byteArray1.constData();
            const char *n2_char_array = byteArray2.constData();

            mpfr_init2(n1, prec_t);
            mpfr_init2(n2, prec_t);
            mpfr_init2(result, prec_t);
            mpfr_set_str(n1, n1_char_array, n1Base, rnd_t);
            mpfr_set_str(n2, n2_char_array, n2Base, rnd_t);

            if(opType == OpType::Add)
                mpfr_add(result, n1, n2, rnd_t);
            else if(opType == OpType::Sub)
                mpfr_sub(result, n1, n2, rnd_t);
            else if(opType == OpType::Mul)
                mpfr_mul(result, n1, n2, rnd_t);
            else if(opType == OpType::Div)
                mpfr_div(result, n1, n2, rnd_t);
            else if(opType == OpType::Mod)
                mpfr_fmod(result, n1, n2, rnd_t);
            else if(opType == OpType::Pow)
                mpfr_pow(result, n1, n2, rnd_t);
            else if(opType == OpType::Root)
                mpfr_rootn_ui(result, n1, mpfr_get_ui(n2, rnd_t), rnd_t);

            mpfr_exp_t exp;
            str = mpfr_get_str(NULL, &exp, resultBase, 0, result, rnd_t);
            result_str = str;

            mpfr_free_str(str);
            mpfr_clears(n1, n2, result, (mpfr_ptr)0);
            mpfr_free_cache();

            insert_dec_sep(result_str, exp);
        }
    }
    else if(appMode == AppMode::BaseConv)
    {
        if(numType == NumType::Integer)
        {
            char *str;
            mpz_t n;

            QByteArray byteArray = n1Str.toLocal8Bit();
            const char *n_char_array = byteArray.constData();

            mpz_init(n);
            mpz_set_str(n, n_char_array, n1Base);

            str = mpz_get_str(NULL, resultBase, n);
            result_str = str;

            free(str);
            mpz_clear(n);
        }
        else if(numType == NumType::FloatingPoint)
        {
            char *str;
            mpfr_t n;
            mpfr_rnd_t rnd_t = MPFR_RNDN;
            mpfr_prec_t prec_t = precision;

            if(roundMode == RoundMode::RNDN)
                rnd_t = MPFR_RNDN;
            else if(roundMode == RoundMode::RNDD)
                rnd_t = MPFR_RNDD;
            else if(roundMode == RoundMode::RNDU)
                rnd_t = MPFR_RNDU;
            else if(roundMode == RoundMode::RNDZ)
                rnd_t = MPFR_RNDZ;
            else if(roundMode == RoundMode::RNDA)
                rnd_t = MPFR_RNDA;

            if(prec_t < MPFR_PREC_MIN)
                prec_t = MPFR_PREC_MIN;
            else if(prec_t > MPFR_PREC_MAX)
                prec_t = MPFR_PREC_MAX;

            QByteArray byteArray = n1Str.toLocal8Bit();
            const char *n_char_array = byteArray.constData();

            mpfr_init2(n, prec_t);
            mpfr_set_str(n, n_char_array, n1Base, rnd_t);

            mpfr_exp_t exp;
            str = mpfr_get_str(NULL, &exp, resultBase, 0, n, rnd_t);
            result_str = str;

            mpfr_free_str(str);
            mpfr_clear(n);
            mpfr_free_cache();

            insert_dec_sep(result_str, exp);
        }
    }

    emit finished(result_str);
}
