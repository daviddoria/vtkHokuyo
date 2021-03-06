#ifndef QRK_IS_USB_COM_H
#define QRK_IS_USB_COM_H

/*!
  \file
  \brief USB ポートかどうかを判別する

  \author Satofumi KAMIMURA

  $Id: IsUsbCom.h 1433 2009-10-20 15:53:11Z satofumi $
*/

#include <vector>
#include <string>


namespace qrk
{
  /*!
    \brief USB ポートの判別クラス
  */
  class IsUsbCom {
  public:
    virtual ~IsUsbCom(void) {
    }


    /*!
      \brief 登録済みのポートのベース名を返す

      Linux 実装で用いる

      \return ポートのベース名
    */
    virtual std::vector<std::string> baseNames(void) const
    {
      std::vector<std::string> dummy;
      return dummy;
    }


    /*!
      \brief 優先表示するドライバ名を返す

      Windows 実装で用いる

      \return ドライバ名
    */
    virtual std::vector<std::string> usbDrivers(void) const
    {
      std::vector<std::string> dummy;
      return dummy;
    }
  };
}

#endif /* !QRK_IS_USB_COM_H */
