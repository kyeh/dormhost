class Modification < ActiveRecord::Base
  #belongs_to :transaction  #This relationship messes up making a new reservation/modification for some reason.
  #validates_presence_of :transaction_id, :comments, :check_in, :check_out, :guests, :approved
  
  def submit_new_modification
    @new_modification = true
  end
  
  def recently_submit_new_modification?
    @new_modification
  end

end
