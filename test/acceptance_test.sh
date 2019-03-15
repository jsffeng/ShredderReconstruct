#/bin/bash

function EXIT 
{
  msg=${1}
  echo "${msg}"
  exit 1
}

function ensure_file_exist
{
  file=${1}
  [ ! -s ${file} ] &&  EXIT "File ${file} doesn't exist, exit!"
}

function ensure_dir_exist
{
  dir=${1}
  [ ! -d ${dir} ] &&  EXIT "Directory ${dir} doesn't exist, exit!"
}

function backup_ascii_files
{
  for file in $(ls ${CURRENT_DIR}/*.ascii 2>/dev/null) 
  do
    mv ${file} ${file}.orig$$
  done
}

function restore_ascii_files
{
  for file in $(ls ${CURRENT_DIR}/*.ascii 2>/dev/null) 
  do
    rm -rf ${file}
  done

  for file in $(ls ${CURRENT_DIR}/*.ascii.orig$$ 2>/dev/null) 
  do
    mv ${file} ${file%%\.orig$$}
  done
}

#### Main Function Start Here ##########

CURRENT_DIR=.
TEST_DIR=${CURRENT_DIR}/test
SAMPLE_DIR=${TEST_DIR}/STinput
RESULT_DIR=${TEST_DIR}/SToutput
SHREDDER_CMD=${CURRENT_DIR}/shredder
UNSHREDDER_CMD=${CURRENT_DIR}/unshredder

ensure_file_exist  ${SHREDDER_CMD}
ensure_file_exist  ${UNSHREDDER_CMD}
ensure_dir_exist  ${SAMPLE_DIR}

if [ -d ${RESULT_DIR} ]
then
  rm -rf ${RESULT_DIR}/*
else
  mkdir -p ${RESULT_DIR}
fi

SHREDDER_INPUT_NAME=full_text.ascii
SHREDDER_OUTPUT_NAME=shredded_text.ascii

UNSHREDDER_INPUT_NAME=${SHREDDER_OUTPUT_NAME}
UNSHREDDER_OUTPUT_NAME=restored_text.ascii

TEST_ORGINAL=original.ascii
TEST_SHREDDED=shredded.ascii
TEST_RESTORED=restored.ascii

declare -i success_t=0
declare -i failure_t=0
declare -i sum=0
declare -i caseid=1
declare -i MAX=5
declare -i i=0

# Backup *.ascii files in $CURRENT_DIR because new *.ascii files will be created & used by the tests 
backup_ascii_files

for SAMPLE_FILE in $(ls ${SAMPLE_DIR}/sample* 2>/dev/null) 
do
  i=0
  
  while (( i < MAX ))
  do
    ((caseid=sum+1))

    if (( caseid < 10 ))
    then
      TESTCASE_DIR=${RESULT_DIR}/TESTCASE0${caseid}
    else
      TESTCASE_DIR=${RESULT_DIR}/TESTCASE${caseid}
    fi

    mkdir -p ${TESTCASE_DIR} 
    
    cp ${SAMPLE_FILE} ${TESTCASE_DIR}/${TEST_ORGINAL}
    cp ${SAMPLE_FILE} ${CURRENT_DIR}/${SHREDDER_INPUT_NAME} 

    # Run shredder to generate $SHREDDER_OUTPUT_NAME from $SHREDDER_INPUT_NAME
    ${SHREDDER_CMD} >/dev/null 2>&1
    
    if [ -s ${SHREDDER_OUTPUT_NAME} ]
    then
      cp ${SHREDDER_OUTPUT_NAME} ${TESTCASE_DIR}/${TEST_SHREDDED}
    fi
    
    # Run unshredder to generate $UNSHREDDER_OUTPUT_NAME from $UNSHREDDER_INPUT_NAME
    ${UNSHREDDER_CMD} >/dev/null 2>&1
    
    if [ -s ${UNSHREDDER_OUTPUT_NAME} ]
    then
      cp ${UNSHREDDER_OUTPUT_NAME} ${TESTCASE_DIR}/${TEST_RESTORED}
    fi

    TESTCASE=${TESTCASE_DIR##*/} 

    # Verify whether the result is expected
    diff  ${TESTCASE_DIR}/${TEST_ORGINAL} ${TESTCASE_DIR}/${TEST_RESTORED} >/dev/null 2>& 1
   
    if [[ $? == 0 ]]
    then
       echo "${TESTCASE}:PASS"
      ((success_t=success_t+1))
    else
       echo "${TESTCASE}:Failed"
      ((failure_t=failure_t+1))
    fi
    
    ((i=i+1))
    ((sum=sum+1))

  done
done

# Remove generated *.ascii by the tests and restore original *.ascii from the backup 
restore_ascii_files

if [[ ${success_t} == ${sum} && ${failure_t} == 0 ]]
then
        MSG1="OK"
else
        MSG1="FAIL"
fi

SUM_MSG="\n--------------------------------\n${sum} Tests: ${success_t} Successes ${failure_t} Failures\n${MSG1}"

echo -e ${SUM_MSG}

echo -e "NOTE: Please find test details under direcotry ${RESULT_DIR}.\n"
