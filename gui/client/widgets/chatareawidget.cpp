#include "chatareawidget.h"

#include "widgets/squarepushbutton.h"
#include <QtGlobal>
#include "core/client/util/dice.h"

QString ChatAreaWidget::getTo()
{
  QString ret = chatBrowserMapReverse[(QTextBrowser*)chatTabs->currentWidget()];
  if(ret=="dm" && !dmTarget.isEmpty())
    ret = dmTarget;
  return ret;
}

ChatAreaWidget::ChatAreaWidget(QWidget *parent) : QWidget(parent)
{
  //layouts
  topLayout = new QHBoxLayout(this);
  mainLayout = new QVBoxLayout();
  diceBarLayout = new QHBoxLayout();
  splitter = new QSplitter(Qt::Vertical);

  //widgets
  chatTabs = new QTabWidget(this);
  chatTabs->tabBar()->setStyleSheet("QTabBar"
                                    "{"
                                    "font-weight: bold"
                                    "}");
  chatEdit = new ChatTextEdit(this);
  sheetSwitchButton = new QPushButton(this);
  sheetSwitchButton->setToolTip("Enable/Disable Alias Sheet Menu");
  sheetSwitchButton->setFocusPolicy(Qt::NoFocus);
  sheetSwitchButton->setFlat(true);
  sheetSwitchButtonStatus = false;
  aliasBox = new QComboBox(this);
  numDiceBox = new QSpinBox(this);
  bonusBox = new QSpinBox(this);
  d2 = new SquarePushButton(this);
  d3 = new SquarePushButton(this);
  d4 = new SquarePushButton(this);
  d6 = new SquarePushButton(this);
  d8 = new SquarePushButton(this);
  d10 = new SquarePushButton(this);
  d12 = new SquarePushButton(this);
  d20 = new SquarePushButton(this);
  d100 = new SquarePushButton(this);

  boldButton = new QPushButton("b");
  italicButton = new QPushButton("i");
  underlineButton = new QPushButton("u");
  colorButton = new ColorSelectButton;
  boldButton->setMaximumWidth(30);
  italicButton->setMaximumWidth(30);
  underlineButton->setMaximumWidth(30);
  colorButton->setMaximumWidth(30);
  colorButton->setMinimumWidth(30);
  boldButton->setStyleSheet("QPushButton{font-weight:bold;}");
  italicButton->setStyleSheet("QPushButton{font-style:italic;}");
  underlineButton->setStyleSheet("QPushButton{text-decoration:underline;}");
  boldButton->setFocusPolicy(Qt::NoFocus);
  italicButton->setFocusPolicy(Qt::NoFocus);
  underlineButton->setFocusPolicy(Qt::NoFocus);
  colorButton->setFocusPolicy(Qt::NoFocus);
  boldButton->setToolTip("Bold");
  italicButton->setToolTip("Italic");
  underlineButton->setToolTip("Underline");
  colorButton->setToolTip("Text Color");

  //set layout
  topLayout->addWidget(splitter);
  splitter->addWidget(chatTabs);
  {
    QWidget* widget = new QWidget(this);
    widget->setLayout(mainLayout);
    splitter->addWidget(widget);
  }
  mainLayout->addLayout(diceBarLayout);
  mainLayout->addWidget(chatEdit);

  //set attributes
  topLayout->setMargin(0);
  mainLayout->setMargin(0);
  diceBarLayout->setMargin(0);

  splitter->setOrientation(Qt::Vertical);
  splitter->setCollapsible(0,false);
  splitter->setCollapsible(1,false);
  splitter->setStretchFactor(0,20);

  numDiceBox->setMinimum(1);
  numDiceBox->setMaximum(999);
  numDiceBox->setToolTip("Number of dice");
  bonusBox->setMinimum(-999);
  bonusBox->setMaximum(999);
  bonusBox->setToolTip("Bonus to roll");

  chatEdit->setMinimumHeight(50);

  numDiceBox->setMaximumWidth(50);
  bonusBox->setMaximumWidth(50);
  aliasBox->setMaximumWidth(500);
  aliasBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  aliasBox->setSizePolicy(QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum));

  //aliasBox->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Maximum);
  splitter->setStretchFactor(0,20);
  splitter->setStretchFactor(1,4);

  //set up button map
  initButton(d2,2);
  initButton(d3,3);
  initButton(d4,4);
  initButton(d6,6);
  initButton(d8,8);
  initButton(d10,10);
  initButton(d12,12);
  initButton(d20,20);
  initButton(d100,100);

  sheetSwitchButton->setIcon(QIcon(":icons/sheet.png"));
  sheetSwitchButton->setMinimumWidth(25);
  sheetSwitchButton->setMaximumWidth(30);

  //set up chat tabs
  chatTabs->setTabsClosable(true);
  addChatTab("Main Channel","all");
  addChatTab("GM","dm");
  chatTabs->tabBar()->tabButton(0,QTabBar::RightSide)->resize(0,0);
  chatTabs->tabBar()->tabButton(1,QTabBar::RightSide)->resize(0,0);
  chatTabs->tabBar()->tabButton(0,QTabBar::RightSide)->hide();
  chatTabs->tabBar()->tabButton(1,QTabBar::RightSide)->hide();
  //chatTabs->tabBar()->tabButton(0,QTabBar::RightSide)->hide();

  //set dice bar layout
  diceBarLayout->addWidget(sheetSwitchButton,0);
  diceBarLayout->addWidget(aliasBox,0);
  diceBarLayout->addWidget(numDiceBox,0,Qt::AlignLeft);
  //diceBarLayout->addWidget(new QLabel("d"));
  //diceBarLayout->addWidget(d2);
  //diceBarLayout->addWidget(d3);
  diceBarLayout->addWidget(d2,0,Qt::AlignLeft);
  diceBarLayout->addSpacing(10);
  diceBarLayout->addWidget(d3,0,Qt::AlignLeft);
  diceBarLayout->addSpacing(10);
  diceBarLayout->addWidget(d4,0,Qt::AlignLeft);
  diceBarLayout->addSpacing(10);
  diceBarLayout->addWidget(d6,0,Qt::AlignLeft);
  diceBarLayout->addSpacing(10);
  diceBarLayout->addWidget(d8,0,Qt::AlignLeft);
  diceBarLayout->addSpacing(10);
  diceBarLayout->addWidget(d10,0,Qt::AlignLeft);
  diceBarLayout->addSpacing(10);
  diceBarLayout->addWidget(d12,0,Qt::AlignLeft);
  diceBarLayout->addSpacing(10);
  diceBarLayout->addWidget(d20,0,Qt::AlignLeft);
  diceBarLayout->addSpacing(10);
  diceBarLayout->addWidget(d100,0,Qt::AlignLeft);
  diceBarLayout->addSpacing(10);
  //diceBarLayout->addWidget(new QLabel("+"));
  diceBarLayout->addWidget(bonusBox,0,Qt::AlignLeft);
  //diceBarLayout->setAlignment(Qt::AlignLeft);
  diceBarLayout->addWidget(new QWidget(),1,Qt::AlignRight);
  diceBarLayout->addWidget(boldButton,0,Qt::AlignRight);
  diceBarLayout->addWidget(italicButton,0,Qt::AlignRight);
  diceBarLayout->addWidget(underlineButton,0,Qt::AlignRight);
  diceBarLayout->addWidget(colorButton,0,Qt::AlignRight);

  //timer
  attentionIcon = QIcon(":icons/attention.png");
  attentionStatus = false;
  attentionTimer = new QTimer(this);
  attentionTimer->setInterval(1000);
  attentionTimer->setSingleShot(false);
  attentionTimer->start();
  connect(attentionTimer,&QTimer::timeout,this,&ChatAreaWidget::attentionToggle);

  //signals

  //chat input
  connect(chatEdit,&ChatTextEdit::typedMessage,this,&ChatAreaWidget::chatPrivate);
  connect(chatEdit,&ChatTextEdit::macroKey,this,&ChatAreaWidget::macroKey);
  connect(chatEdit,&QTextEdit::textChanged,this,&ChatAreaWidget::typedPrivate);

  //chat browser
  connect(chatTabs,&QTabWidget::currentChanged,this,&ChatAreaWidget::tabSelected);
  connect(chatTabs,&QTabWidget::tabCloseRequested,this,&ChatAreaWidget::closeTab);

  //dice buttons
  connect(d2,&QAbstractButton::clicked,this,&ChatAreaWidget::diceButtonPressed);
  connect(d3,&QAbstractButton::clicked,this,&ChatAreaWidget::diceButtonPressed);
  connect(d4,&QAbstractButton::clicked,this,&ChatAreaWidget::diceButtonPressed);
  connect(d6,&QAbstractButton::clicked,this,&ChatAreaWidget::diceButtonPressed);
  connect(d8,&QAbstractButton::clicked,this,&ChatAreaWidget::diceButtonPressed);
  connect(d10,&QAbstractButton::clicked,this,&ChatAreaWidget::diceButtonPressed);
  connect(d12,&QAbstractButton::clicked,this,&ChatAreaWidget::diceButtonPressed);
  connect(d20,&QAbstractButton::clicked,this,&ChatAreaWidget::diceButtonPressed);
  connect(d100,&QAbstractButton::clicked,this,&ChatAreaWidget::diceButtonPressed);

  //alias sheet button
  connect(sheetSwitchButton,&QPushButton::clicked,this,&ChatAreaWidget::sheetButtonPressed);
  //alias
  connect(aliasBox,qOverload<int>(&QComboBox::currentIndexChanged),this,&ChatAreaWidget::selectAlias);
  connect(AliasLib::get(),&AliasLib::selectedAliasesChanged,this,&ChatAreaWidget::updateAliasList);
  connect(AliasLib::get(),&AliasLib::currentAliasChanged,this,&ChatAreaWidget::updateAlias);
  connect(AliasLib::get(),&AliasLib::currentAliasIndexChanged,this,&ChatAreaWidget::updateAlias);
  //chat tabs
  connect(chatTabs,&QTabWidget::currentChanged,this,&ChatAreaWidget::loadTabAlias);

  //formatting
  connect(boldButton,&QAbstractButton::clicked,chatEdit,&ChatTextEdit::toggleBold);
  connect(italicButton,&QAbstractButton::clicked,chatEdit,&ChatTextEdit::toggleItalic);
  connect(underlineButton,&QAbstractButton::clicked,chatEdit,&ChatTextEdit::toggleUnderline);
  connect(colorButton,&ColorSelectButton::finalColorChanged,chatEdit,&ChatTextEdit::setTextColor);
  connect(chatEdit,&QTextEdit::currentCharFormatChanged,this,&ChatAreaWidget::charFormatChanged);
  connect(orpg::Settings::get(),&orpg::Settings::fontChanged,this,&ChatAreaWidget::setFont);
  connect(colorButton,&ColorSelectButton::rightClicked,this,&ChatAreaWidget::updateAlias);

  //night mode

  setFont(orpg::Settings::get()->font);
  updateAliasList();
}

QSplitter *ChatAreaWidget::getSplitter()
{
  return splitter;
}

void ChatAreaWidget::setPlayers(QVector<Player> players)
{
  int dmCtr=0;
  for(int i=0;i<players.size();i++)
  {
    idMap.insert(players[i].id,players[i].name);
    if(players[i].role==ROLE_GM)
    {
      dmCtr++;
      dmTarget = players[i].id;
    }
  }
  if(dmCtr!=1)
    dmTarget.clear();
  for(int i=2;i<chatTabs->count();i++)
  {
    if(chatBrowserMapReverse.contains((QTextBrowser*)chatTabs->widget(i)))
    {
      QString id = chatBrowserMapReverse[(QTextBrowser*)chatTabs->widget(i)];
      chatTabs->setTabText(i, getNameByID(id));
    }
  }
}

void ChatAreaWidget::updateAliasList()
{
  aliasBox->clear();
  foreach(const Alias* alias,AliasLib::get()->getSelectedAliases())
  {
    aliasBox->addItem(alias->getName());
  }
  aliasBox->setCurrentIndex(AliasLib::get()->getCurrentAliasIndex());

}

void ChatAreaWidget::openWhisperTab(quint32 id)
{
  QString str = QString::number(id);
  if(str!=dmTarget)
  {
    openTab(str);
    chatTabs->setCurrentWidget(chatBrowserMap[str]);
  }
  else
  {
    chatTabs->setCurrentWidget(chatBrowserMap["dm"]);
  }
}

void ChatAreaWidget::setID(quint32 id)
{
  myID = QString::number(id);
}

void ChatAreaWidget::setFont(const QFont &font)
{
  if(this->font != font)
  {
    this->font = font;
    chatEdit->setFont(font);
    foreach(QTextBrowser* browser, chatBrowserMapReverse.keys().toSet())
    {
      browser->setFont(font);
    }
  }
}

void ChatAreaWidget::sheetButtonPressed()
{
  sheetSwitchButtonStatus = !sheetSwitchButtonStatus;
  sheetSwitchButton->setDown(sheetSwitchButtonStatus);
  emit setSheetsVisible(sheetSwitchButtonStatus);
}

void ChatAreaWidget::addChatTab(const QString &title, const QString &id)
{
  //prevent double-adding
  if(chatBrowserMap.contains(id))
  {
    return;
  }
  QTextBrowser* newTab = new QTextBrowser(this);
  newTab->setFont(font);
  chatTabs->addTab(newTab,title);
  chatBrowserMap.insert(id,newTab);
  chatBrowserMapReverse.insert(newTab,id);
}

void ChatAreaWidget::openTab(QString &id)
{
  if(chatBrowserMap.contains(id))
  {
    if(chatTabs->indexOf(chatBrowserMap[id])==-1)
    {
      chatTabs->addTab(chatBrowserMap[id],getNameByID(id));
    }
  }
  else
  {
    addChatTab(getNameByID(id),id);
  }
}

QTextBrowser *ChatAreaWidget::getChatBrowser(const QString &from, const QString &to)
{
  QString id;
  if(to.isEmpty() || to=="all")
    id = "all";
  else if(to =="dm")
    id = "dm";
  else if(from.isEmpty())
    id = "all";
  else
    id = from;
  if(id==myID)
    id = to;
  if(id==dmTarget)
    id = "dm";
  openTab(id);
  return chatBrowserMap[id];
}

QString ChatAreaWidget::getNameByID(const QString &id)
{
  if(idMap.contains(id))
    return idMap[id]+" ("+id+")";
  else
    return "("+id+")";
}

void ChatAreaWidget::initButton(QPushButton *button, int value)
{
  QString valueString = QString::number(value);
  button->setStyleSheet("QPushButton"
                        "{"
                        "color: black invert;"
                        "border-image: url(:/icons/d"+valueString+".png) 0 0 0 0 stretch stretch;"
                        "font-size: 8px;"
                        "margin-right: 3px;"
                        "}");
  button->setText(valueString);
  button->setToolTip("d"+valueString);
  button->setFlat(true);
  button->setMinimumHeight(20);
  button->setMaximumHeight(20);
  button->setMaximumWidth(20);
  button->setMaximumWidth(20);
  diceMap.insert((QObject*)button,value);
}

void ChatAreaWidget::selectAlias(int index)
{
  AliasLib::get()->setCurrentAliasIndex(index);
  chatBrowserAliasMap.insert(chatTabs->currentWidget(),index);
}

void ChatAreaWidget::updateAlias()
{
  aliasBox->setCurrentIndex(AliasLib::get()->getCurrentAliasIndex());
  QColor c(AliasLib::get()->getCurrentAlias()->getColor());
  colorButton->setColor(c,false);
  chatEdit->overwriteTextColor(c);
}

void ChatAreaWidget::chatPrivate(QString text)
{
  if(!text.isEmpty())
    emit chat(text,getTo());
}

void ChatAreaWidget::macroKey(int index)
{
  if(index>=0 && index < orpg::Settings::get()->macro.size())
    emit chat(orpg::Settings::get()->macro[index],getTo());
}

void ChatAreaWidget::typedPrivate()
{
  if(!chatEdit->sendKeyPressed)
    return;
  if(chatEdit->toPlainText().isEmpty())
    emit idle();
  else
    emit typed();
}

void ChatAreaWidget::diceButtonPressed(bool)
{
  QObject* sender = QObject::sender();
  if(!diceMap.contains(sender))
    return;
  int dice = diceMap[sender];
  int sum = bonusBox->text().toInt();
  int numDice = numDiceBox->text().toInt();
  if(numDice<=0)
    return;
  QString textSent = QString::number(numDice)+"d"+QString::number(dice)+":";
  for(int i=0;i<numDice-1;i++)
  {
    int rolled = Dice::roll(dice);
    textSent.append(QString::number(rolled)+",");
    sum += rolled;
  }
  int rolled = Dice::roll(dice);
  textSent.append(QString::number(rolled));
  int bonus = bonusBox->text().toInt();
  if(bonus<0)
    textSent.append(" - "+QString::number(qAbs(bonus)));
  else if(bonus>0)
    textSent.append(" + "+QString::number(qAbs(bonus)));
  sum += rolled;
  textSent.append(" = "+QString::number(sum));
  emit chatPrivate(textSent);
}

void ChatAreaWidget::loadTabAlias(int index)
{
  if(!orpg::Settings::get()->aliasPerChannel)
    return;
  QWidget* widget = chatTabs->widget(index);
  int aliasIndex;
  if(chatBrowserAliasMap.contains(widget))
  {
    aliasIndex =  chatBrowserAliasMap[widget];
  }
  else
    aliasIndex = 0;
  if(aliasIndex >= aliasBox->count())
    aliasIndex = 0;
  AliasLib::get()->setCurrentAliasIndex(aliasIndex);
  aliasBox->setCurrentIndex(aliasIndex);
}

void ChatAreaWidget::printErrorMessage(QString text)
{
  getChatBrowser("all")->append("<font color=red>"+text.toHtmlEscaped()+"</font>\n");
  emit notify();
}

void ChatAreaWidget::printStatusMessage(QString text)
{
  getChatBrowser("all")->append("<font color=orange>"+text.toHtmlEscaped()+"</font>\n");
  emit notify();
}

void ChatAreaWidget::printChatMessage(const orpg::ChatMessage& chatMessage)
{
  QTextBrowser* tb = getChatBrowser(chatMessage.senderId(),chatMessage.to());
  if(tb==nullptr)
    return;
  QString idpart;
  if(chatMessage.hasSenderId())
    idpart = "(" + chatMessage.senderId() + ") ";
  tb->append("<b>" + idpart + chatMessage.name() + "</b>: <font color='" + chatMessage.color() + "'>"+chatMessage.text().replace("\n","<br/>")+"</font>\n" );
  if(tb!=chatTabs->currentWidget())
  {
    emit inactiveChannelMessage();
    attentionSet.insert(tb);
    chatTabs->tabBar()->setTabIcon(chatTabs->indexOf(tb), attentionIcon);
    chatTabs->tabBar()->setTabTextColor(chatTabs->indexOf(tb),QColor(255,0,0));
  }
  emit notify();
}

void ChatAreaWidget::tabSelected(int index)
{
  attentionSet.remove(chatTabs->widget(index));
  chatTabs->tabBar()->setTabIcon(index, QIcon());
  chatTabs->tabBar()->setTabTextColor(index,QColor());
}

void ChatAreaWidget::closeTab(int index)
{
  chatTabs->removeTab(index);
}

void ChatAreaWidget::charFormatChanged(const QTextCharFormat &format)
{
  TextFormat tf(format);
  boldButton->setDown(tf.getBold());
  italicButton->setDown(tf.getItalic());
  underlineButton->setDown(tf.getUnderline());
  if(format.foreground()!=Qt::NoBrush)
    colorButton->setColor(tf.getTextColor(),false);
  else
    colorButton->setColor(AliasLib::get()->getCurrentAlias()->getColor(),false);
}

void ChatAreaWidget::attentionToggle()
{
  if(attentionStatus)
  {
    foreach(QWidget* widget, attentionSet)
    {
      int index = chatTabs->indexOf(widget);
      if(index>=0)
      {
        chatTabs->tabBar()->setTabTextColor(index,QColor(255,0,0));
      }
    }
  }
  else
  {
    foreach(QWidget* widget, attentionSet)
    {
      int index = chatTabs->indexOf(widget);
      if(index>=0)
        chatTabs->tabBar()->setTabTextColor(index,QColor());
    }
  }
  attentionStatus = !attentionStatus;
}
