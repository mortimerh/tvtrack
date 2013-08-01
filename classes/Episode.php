<?
class Episode{
	private $id;
	private $seasonId;
	private $number;
	private $name;
	private $airdate;
	private $tvdbId;
	private $tvrageId;

	public function __construct($id, $seasonId, $number, $name, $airdate, $tvdbId, $tvrageId){
		$this->id = $id;
		$this->seasonId = $seasonId;
		$this->number = $number;
		$this->name = $name;
		$this->airdate = $airdate;
		$this->tvdbId = $tvdbId;
		$this->tvrageId = $tvrageId;
	}

  /**
   * Gets the value of id.
   * @return int
   */
  public function getId()
  {
      return $this->id;
  }

  /**
   * Sets the value of id.
   * @param int $id the id
   * @return self
   */
  public function setId($id)
  {
      $this->id = $id;

      return $this;
  }

  /**
   * Gets the value of seasonId.
   * @return int
   */
  public function getSeasonId()
  {
      return $this->seasonId;
  }

  /**
   * Sets the value of seasonId.
   * @param int $seasonId the seasonId
   * @return self
   */
  public function setSeasonId($seasonId)
  {
      $this->seasonId = $seasonId;
      return $this;
  }

  /**
   * Gets the value of number.
   * @return int
   */
  public function getNumber()
  {
      return $this->number;
  }

  /**
   * Sets the value of number.
   * @param int $number the number
   * @return self
   */
  public function setNumber($number)
  {
      $this->number = $number;
      return $this;
  }

  /**
   * Gets the value of name.
   * @return string
   */
  public function getName()
  {
      return $this->name;
  }

  /**
   * Sets the value of name.
   * @param string $name the name
   * @return self
   */
  public function setName($name)
  {
      $this->name = $name;
      return $this;
  }

  /**
   * Gets the value of airdate (YYYY-MM-DD).
   * @return string
   */
  public function getAirdate()
  {
      return $this->airdate;
  }

  /**
   * Sets the value of airdate (YYYY-MM-DD).
   * @param string $airdate the airdate
   * @return self
   */
  public function setAirdate($airdate)
  {
      $this->airdate = $airdate;
      return $this;
  }

  /**
   * Gets the value of tvdbId.
   * @return int
   */
  public function getTvdbId()
  {
      return $this->tvdbId;
  }

  /**
   * Sets the value of tvdbId.
   * @param int $tvdbId the tvdbId
   * @return self
   */
  public function setTvdbId($tvdbId)
  {
      $this->tvdbId = $tvdbId;
      return $this;
  }

  /**
   * Gets the value of tvrageId.
   * @return int
   */
  public function getTvrageId()
  {
      return $this->tvrageId;
  }

  /**
   * Sets the value of tvrageId.
   * @param int $tvrageId the tvrageId
   * @return self
   */
  public function setTvrageId($tvrageId)
  {
      $this->tvrageId = $tvrageId;
      return $this;
  }
}
?>