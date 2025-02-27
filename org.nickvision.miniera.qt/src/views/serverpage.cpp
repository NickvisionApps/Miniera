#include "views/serverpage.h"

using namespace Nickvision::Miniera::Shared::Controllers;

namespace Ui
{
    class ServerPage
    {
    public:
        void setupUi(Nickvision::Miniera::Qt::Views::ServerPage* parent)
        {

        }
    };
}

namespace Nickvision::Miniera::Qt::Views
{
    ServerPage::ServerPage(const std::shared_ptr<ServerViewController>& controller, QWidget* parent)
        : QWidget{ parent },
        m_ui{ new Ui::ServerPage() },
        m_controller{ controller }
    {
        //Load Ui
        m_ui->setupUi(this);
    }

    ServerPage::~ServerPage()
    {
        delete m_ui;
    }
}