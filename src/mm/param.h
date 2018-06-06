/* The following names are synonyms for the variables in the input message. */
#define addr		mm_in.m1_p1
#define exec_name	mm_in.m1_p1
#define exec_len	mm_in.m1_i1
#define func		mm_in.m6_f1
#define grpid		(gid_t) mm_in.m1_i1
#define namelen		mm_in.m1_i1
#define pid		mm_in.m1_i1
#define seconds		mm_in.m1_i1
#define sig		mm_in.m6_i1
#define stack_bytes	mm_in.m1_i2
#define stack_ptr	mm_in.m1_p2
#define status		mm_in.m1_i1
#define usr_id		(uid_t) mm_in.m1_i1
#define request		mm_in.m2_i2
#define taddr		mm_in.m2_l1
#define data		mm_in.m2_l2
#define sig_nr		mm_in.m1_i2
#define sig_nsa		mm_in.m1_p1
#define sig_osa		mm_in.m1_p2
#define sig_ret		mm_in.m1_p3
#define sig_set		mm_in.m2_l1
#define sig_how		mm_in.m2_i1
#define sig_flags		mm_in.m2_i2
#define sig_context	mm_in.m2_p1
#ifdef _SIGMESSAGE
#define sig_msg		mm_in.m1_i1
#endif
#define reboot_flag	mm_in.m1_i1
#define reboot_code	mm_in.m1_p1
#define reboot_size	mm_in.m1_i2

/* The following names are synonyms for the variables in the output message. */
#define reply_type      mm_out.m_type
#define reply_i1        mm_out.m2_i1
#define reply_p1        mm_out.m2_p1
#define ret_mask	mm_out.m2_l1 	

#ifdef MRT
#define rtt_opcode	mm_in.m1_i1		/* MRTTASK operation code 		*/

#define rtt_harmonic	mm_in.m1_i2		/* MRT_START harmonic 			*/
#define rtt_irq		mm_in.m1_i2		/* MRT_SETIATTR/MRT_GETIATTR irq number */
#define rtt_pid		mm_in.m1_i2		/* MRT_GETPATTR PID number 		*/
#define rtt_flags 	mm_in.m1_i2		/* MRT_SETSVAL flags 			*/
#define rtt_semid		mm_in.m1_i2		/* Semaphore ID number 			*/
#define rtt_msgqid	mm_in.m1_i2		/* Message Queue ID number 		*/

#define rtt_refresh	mm_in.m1_i3		/* MRT_START refresh 			*/

#define rtt_irqattr	mm_in.m1_p1		/* MRT_SETIATTR/MRT_GETIATTR		*/
#define rtt_irqstat	mm_in.m1_p1		/* MRT_GETISTAT				*/
#define rtt_irqint	mm_in.m1_p1		/* MRT_GETIINT				*/
#define rtt_sysstat	mm_in.m1_p1		/* MRT_GETSSTAT system statistics   */
#define rtt_sysval	mm_in.m1_p1		/* MRT_GETSVAL  system values       */
#define rtt_pattrib	mm_in.m1_p1		/* MRT_SETPATTR process attributes 	*/
#define rtt_pstats	mm_in.m1_p1		/* MRT_SETPSTAT/MRT_GETPSTAT  statistics 	*/
#define rtt_pint		mm_in.m1_p1		/* MRT_GETINT process internal vals */
#define rtt_sattr		mm_in.m1_p1		/* MRT_SEMALLOC/MRT_GETSEMATTR	*/
#define rtt_sstat		mm_in.m1_p1		/* MRT_GETSEMSTAT				*/
#define rtt_sint		mm_in.m1_p1		/* MRT_GETSEMINT				*/
#define rtt_sname		mm_in.m1_p1		/* MRT_GETSEMID				*/
#define rtt_mqstat	mm_in.m1_p1		/* MRT_GETMQSTAT				*/

#endif
