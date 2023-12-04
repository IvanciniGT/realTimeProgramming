#ifndef CARDVALIDATOR_CARD_VALIDATION_THREAD_POOL_H
#define CARDVALIDATOR_CARD_VALIDATION_THREAD_POOL_H

void configure_validation_thread_pool() ;
void new_card_detected(u_int8_t cardId[CARD_ID_SIZE]) ;
void clean_up_validation_thread_pool() ;


#endif //CARDVALIDATOR_CARD_VALIDATION_THREAD_POOL_H