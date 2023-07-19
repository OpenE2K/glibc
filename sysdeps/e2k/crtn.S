/* Copyright (c) 2016 ZAO "MCST". All rights reserved. */

/*@HEADER_BEGINS*/

/*@HEADER_ENDS*/
/*@_init_EPILOG_BEGINS*/
    .section    ".init", "ax", @progbits
    return  %ctpr3
    ct      %ctpr3
/*@_init_EPILOG_ENDS*/
/*@_fini_EPILOG_BEGINS*/
    .section    ".fini", "ax", @progbits
    return  %ctpr3
    ct      %ctpr3
/*@_fini_EPILOG_ENDS*/
/*@TRAILER_BEGINS*/
/*@TRAILER_ENDS*/
