{
 "cells": [
  {
   "cell_type": "code",
   "execution_count": 36,
   "metadata": {},
   "outputs": [
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "55,\n",
      "74,\n",
      "79,\n",
      "68,\n",
      "70,\n"
     ]
    }
   ],
   "source": [
    "charlst = ['!','\"','#','$','%','&','´','(',')','*','+',',','-','.','/','0','1','2','3','4','5','6','7','8','9',':',';','<','=','>','?','@','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','[','\\\\',']','^','_','´','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','{','|','}','~',' ']\n",
    "\n",
    "def splitw(w):\n",
    "    return[c for c in w]\n",
    "\n",
    "inp = input('Enter a string to translate: ')\n",
    "blist = []\n",
    "for l in splitw(inp):\n",
    "    blist.append(charlst.index(l))\n",
    "for s in blist:\n",
    "    print(f'{int(s) +1},')\n"
   ]
  }
 ],
 "metadata": {
  "kernelspec": {
   "display_name": "Python 3.10.5 64-bit",
   "language": "python",
   "name": "python3"
  },
  "language_info": {
   "codemirror_mode": {
    "name": "ipython",
    "version": 3
   },
   "file_extension": ".py",
   "mimetype": "text/x-python",
   "name": "python",
   "nbconvert_exporter": "python",
   "pygments_lexer": "ipython3",
   "version": "3.10.5"
  },
  "orig_nbformat": 4,
  "vscode": {
   "interpreter": {
    "hash": "e7370f93d1d0cde622a1f8e1c04877d8463912d04d973331ad4851f04de6915a"
   }
  }
 },
 "nbformat": 4,
 "nbformat_minor": 2
}
