import email.utils
import subprocess

class CommiterInfo(object):
  """ Commiter description structure """
  def __init__(self, shortlog_line):
    fields = shortlog_line.split('\t')
    name_email = email.utils.parseaddr(fields[1])
    self.commit_count = int(fields[0])
    self.name = name_email[0]
    self.email = name_email[1]

def BranchCommiters():
  # Call git and parse it's output
  git_shortlog = subprocess.check_output(["git", "shortlog", "-sne"],
                                       stderr=subprocess.PIPE)
  return map(lambda s: CommiterInfo(s), git_shortlog.splitlines())
