# -*- coding: utf-8 -*-

import glob
import os
import re
import subprocess

class Corrector(object):
    def __init__(self, max_time: int= 1) -> None:
        self.__max_time = max_time
        pass
    
    def __list_test_files(self):
        files = glob.glob(f'./tests/*.in')
        files.sort()
        return files
    
    def __compile(self):
        out = subprocess.Popen(['make'], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        stdout, stderr = out.communicate()
        stdout = stdout.decode('utf8').strip()
        stderr = '' if stderr is None else stderr.decode('utf8').strip()

        if re.search(r'\d errors? generated\.', stdout):
            return 'error'

        if stdout or stderr:
            print(stdout)
            return 'warning'
        return 'ok'

    def __compare_outputs(self, file1, file2):
        data_file1 = open(file1, 'r').read().replace(' ', '').replace('\n', '')
        data_file2 = open(file2, 'r').read().replace(' ', '').replace('\n', '')
        if data_file1 == data_file2:
            return True
        return False
    
    def __run_test(self, test_case):
        p = subprocess.Popen(f'./exe < {test_case} > output.out', shell=True)
        try:
            p.wait(self.__max_time)
        except subprocess.TimeoutExpired:
            p.kill()
            return False
        return True

    def __check_response(self, input_test_cases):
        correct = 0
        for input_test_case in input_test_cases:
            print(f'    {os.path.basename(input_test_case)}', end=' ')
            if not self.__run_test(input_test_case):
                print('\x1b[6;30;41m FAIL \x1b[0m')
            if self.__compare_outputs('output.out', input_test_case.replace('.in', '.out')):
                print('\x1b[6;30;42m OK \x1b[0m')
                correct += 1
            else:
                print('\x1b[6;30;41m ER \x1b[0m')
                data_file1 = open(input_test_case.replace('.in', '.out'), 'r').read()
                data_file2 = open('output.out', 'r').read()
                if data_file1 == data_file2:
                    return True
        return 10.0 * correct / len(input_test_cases)
    
    def run(self):
        flag = False
        print(f'Analisando atividade:')

        if not os.path.exists('./Makefile'):
            print('  Arquivo "Makefile" não existe!')
            flag = True
        
        if not os.path.exists('./tests'):
            print('  A pasta de arquivos de teste não existe!')
            flag = True
        
        input_cases = self.__list_test_files()
        if len(input_cases) == 0:
            print('  É necessário pelo menos um arquivo de teste (arquivo ".in")!')
            flag = True
        for f in input_cases:
            if not os.path.exists(f.replace('.in', '.out')):
                flag = True
                print(f'  O arquivo de teste {os.path.basename(f)} não possui um arquivo de saída correspondente (arquivo ".out")!')
        if flag:
            return
        compilation_response = self.__compile()
        if compilation_response == 'error':
            grade = 0.0
            print('\x1b[6;30;43m  Erro de compilação!  \x1b[0m')
        else:
            factor = 1.0
            if compilation_response == 'warning':
                print('\x1b[6;30;43m  Seu código possui Warnings. A nota será reduzida pela metade!  \x1b[0m')
                factor = 0.5
            grade = self.__check_response(input_cases) * factor
        print(f'Nota na atividade: {grade:.2f}')

if __name__ == '__main__':
    status = Corrector(
        max_time=1
    ).run()

