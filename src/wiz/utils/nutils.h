namespace wiznet
{
  class nutils
  {
  public:
    //static int sendmail(std::string to, std::string from, std::string subject, std::string message);
    static int sendmail(const char *to, const char *from, const char *subject, const char *message);
  };
}
